//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 Glenn De Jonghe
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "online/http_request.hpp"

#include "config/user_config.hpp"
#include "online/request_manager.hpp"
#include "utils/constants.hpp"
#include "utils/translation.hpp"

#ifdef WIN32
#  include <winsock2.h>
#endif

#include <curl/curl.h>

#include <assert.h>


namespace Online
{
    /** Creates a HTTP(S) request that will have a raw string as result. (Can 
     *  of course be used if the result doesn't matter.)
     *  \param manage_memory whether or not the RequestManager should take care of
     *         deleting the object after all callbacks have been done.
     *  \param priority by what priority should the RequestManager take care of 
     *         this request.
     */
    HTTPRequest::HTTPRequest(bool manage_memory, int priority)
               : Request(manage_memory, priority, 0)
    {
        init();
    }   // HTTPRequest

    // ------------------------------------------------------------------------
    /** This constructor configures this request to save the data in a flie.
     *  \param filename Name of the file to save the data to.
     *  \param manage_memory whether or not the RequestManager should take care of
     *         deleting the object after all callbacks have been done.
     *  \param priority by what priority should the RequestManager take care of 
     *         this request.
     */
    HTTPRequest::HTTPRequest(const std::string &filename, bool manage_memory, 
                                       int priority)
               : Request(manage_memory, priority, 0)
    {
        // A http request should not even be created when internet is disabled
        assert(UserConfigParams::m_internet_status == 
               RequestManager::IPERM_ALLOWED);
        assert(filename.size()>0);
        init();
        m_filename = file_manager->getAddonsFile(filename);
    }   // HTTPRequest(filename ...)

    // ------------------------------------------------------------------------
    /** Char * needs a separate constructor, otherwise it will be considered
     *  to be the no-filename constructor (char* -> bool).
     */
    HTTPRequest::HTTPRequest(const char* const filename, bool manage_memory, 
                                       int priority)
               : Request(manage_memory, priority, 0)
    {
        // A http request should not even be created when internet is disabled
        assert(UserConfigParams::m_internet_status == 
               RequestManager::IPERM_ALLOWED);
        init();
        m_filename = file_manager->getAddonsFile(filename);
    }   // HTTPRequest(filename ...)

    // ------------------------------------------------------------------------
    /** Initialises all member variables.
     */
    void HTTPRequest::init()
    {
        m_url           = "";
        m_string_buffer = "";
        m_filename      = "";
        m_parameters    = "";
        m_curl_code     = CURLE_OK;
        m_progress.setAtomic(0);
    }   // init

    // ------------------------------------------------------------------------
    /** A handy shortcut that appends the given path to the URL of the 
     *  mutiplayer server.
     *  \param path The path to add to the server.
     */
    void HTTPRequest::setServerURL(const std::string& path)
    {
        setURL((std::string)UserConfigParams::m_server_multiplayer+path);
    }   // setServerURL

    // ------------------------------------------------------------------------
    /** A handy shortcut that appends the given path to the URL of the addons
     *  server.
     *  \param path The path to add to the server.
     */
     void HTTPRequest::setAddonsURL(const std::string& path)
     {
        setURL((std::string)UserConfigParams::m_server_addons
                 + "/" + path);
     }   // set AddonsURL
    // ------------------------------------------------------------------------
    /** Checks the request if it has enough (correct) information to be
     *  executed (and thus allowed to add to the queue).
     */
    bool HTTPRequest::isAllowedToAdd()
    {
        return Request::isAllowedToAdd() && m_url.substr(0, 5) == "http:";
    }   // isAllowedToAdd

    // ------------------------------------------------------------------------
    /** Sets up the curl data structures.
     */
    void HTTPRequest::prepareOperation()
    {
        m_curl_session = curl_easy_init();
        if(!m_curl_session)
        {
            Log::error("HTTPRequest::prepareOperation",
                       "LibCurl session not initialized.");
            return;
        }

        curl_easy_setopt(m_curl_session, CURLOPT_URL, m_url.c_str());
        curl_easy_setopt(m_curl_session, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(m_curl_session, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(m_curl_session, CURLOPT_PROGRESSDATA, this);
        curl_easy_setopt(m_curl_session, CURLOPT_PROGRESSFUNCTION,
                                         &HTTPRequest::progressDownload);
        curl_easy_setopt(m_curl_session, CURLOPT_CONNECTTIMEOUT, 20);
        curl_easy_setopt(m_curl_session, CURLOPT_LOW_SPEED_LIMIT, 10);
        curl_easy_setopt(m_curl_session, CURLOPT_LOW_SPEED_TIME, 20);
        //curl_easy_setopt(m_curl_session, CURLOPT_VERBOSE, 1L);
        if(m_filename.size()==0)
        {
            //https
            struct curl_slist *chunk = NULL;
            chunk = curl_slist_append(chunk, "Host: api.stkaddons.net");
            curl_easy_setopt(m_curl_session, CURLOPT_HTTPHEADER, chunk);
            curl_easy_setopt(m_curl_session, CURLOPT_CAINFO, 
                file_manager->getAsset("web.tuxfamily.org.pem").c_str());
            curl_easy_setopt(m_curl_session, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(m_curl_session, CURLOPT_SSL_VERIFYHOST, 0L);
        }
    }   // prepareOperation

    // ------------------------------------------------------------------------
    /** The actual curl download happens here.
     */
    void HTTPRequest::operation()
    {
        if(!m_curl_session)
            return;

        FILE *fout = NULL;
        if(m_filename.size()>0)
        {
            fout = fopen((m_filename+".part").c_str(), "wb");

            if(!fout)
            {
                Log::error("HTTPRequest",
                           "Can't open '%s' for writing, ignored.",
                           (m_filename+".part").c_str());
                return;
            }
            curl_easy_setopt(m_curl_session,  CURLOPT_WRITEDATA,     fout  );
            curl_easy_setopt(m_curl_session,  CURLOPT_WRITEFUNCTION, fwrite);
        }
        else
        {
            curl_easy_setopt(m_curl_session, CURLOPT_WRITEDATA, 
                             &m_string_buffer);
            curl_easy_setopt(m_curl_session, CURLOPT_WRITEFUNCTION, 
                             &HTTPRequest::writeCallback);
        }

        // All parameters added have a '&' added 
        if(m_parameters.size()>0)
        {
            m_parameters.erase(m_parameters.size()-1);
        }

        if(m_parameters.size()==0)
            Log::info("HTTPRequest", "Downloading %s", m_url.c_str());
        else
            Log::info("HTTPRequest", "Sending %s to %s",
                       m_parameters.c_str(), m_url.c_str());
        curl_easy_setopt(m_curl_session, CURLOPT_POSTFIELDS,
                         m_parameters.c_str());
        std::string uagent( std::string("SuperTuxKart/") + STK_VERSION );
            #ifdef WIN32
                    uagent += (std::string)" (Windows)";
            #elif defined(__APPLE__)
                    uagent += (std::string)" (Macintosh)";
            #elif defined(__FreeBSD__)
                    uagent += (std::string)" (FreeBSD)";
            #elif defined(linux)
                    uagent += (std::string)" (Linux)";
            #else
                    // Unknown system type
            #endif
        curl_easy_setopt(m_curl_session, CURLOPT_USERAGENT, uagent.c_str());

        m_curl_code = curl_easy_perform(m_curl_session);
        Request::operation();

        if(fout)
        {
            fclose(fout);
            if(m_curl_code==CURLE_OK)
            {
                if(UserConfigParams::logAddons())
                    Log::info("HTTPRequest", "Download successful.");
                // The behaviour of rename is unspecified if the target
                // file should already exist - so remove it.
                bool ok = file_manager->removeFile(m_filename);
                if(!ok)
                {
                    Log::error("addons",
                               "Could not removed existing addons.xml file.");
                    m_curl_code = CURLE_WRITE_ERROR;
                }
                int ret = rename((m_filename+".part").c_str(), 
                                 m_filename.c_str()           );
                // In case of an error, set the status to indicate this
                if(ret!=0)
                {
                    Log::error("addons",
                               "Could not rename downloaded addons.xml file!");
                    m_curl_code = CURLE_WRITE_ERROR;
                }
            }   // m_curl_code ==CURLE_OK
        }   // if fout
    }   // operation

    // ------------------------------------------------------------------------
    /** Cleanup once the download is finished. The value of progress is 
     *  guaranteed to be >=0 and <1 while the download is in progress, and
     *  will only be set to 1 on a successfull finish here.
     */
    void HTTPRequest::afterOperation()
    {
        if(m_curl_code == CURLE_OK)
            setProgress(1.0f);
        else
            setProgress(-1.0f);
        Request::afterOperation();
        curl_easy_cleanup(m_curl_session);
    }   // afterOperation

    // ------------------------------------------------------------------------
    /** Callback from curl. This stores the data received by curl in the
     *  buffer of this request.
     *  \param content Pointer to the data received by curl.
     *  \param size Size of one block.
     *  \param nmemb Number of blocks received.
     *  \param userp Pointer to the user buffer.
     */
    size_t HTTPRequest::writeCallback(void *contents, size_t size,
                                      size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }   // writeCallback

    // ----------------------------------------------------------------------------
    /** Callback function from curl: inform about progress. It makes sure that
     *  the value reported by getProgress () is <1 while the download is still
     *  in progress.
     *  \param clientp
     *  \param download_total Total size of data to download.
     *  \param download_now   How much has been downloaded so far.
     *  \param upload_total   Total amount of upload.
     *  \param upload_now     How muc has been uploaded so far.
     */
    int HTTPRequest::progressDownload(void *clientp,
                                      double download_total, double download_now,
                                      double upload_total,   double upload_now)
    {
        HTTPRequest *request = (HTTPRequest *)clientp;

        RequestManager* request_manager = RequestManager::get();

        // Check if we are asked to abort the download. If so, signal this
        // back to libcurl by returning a non-zero status.
        if(request_manager->getAbort() || request->isCancelled() )
        {
            // Indicates to abort the current download, which means that this
            // thread will go back to the mainloop and handle the next request.
            return 1;
        }

        float f;
        if(download_now < download_total)
        {
            f = (float)download_now / (float)download_total;
            // In case of floating point rouding errors make sure that
            // 1.0 is only reached when downloadFileInternal is finished
            if (f>=1.0f) f=0.99f;
        }
        else
        {
            // Don't set progress to 1.0f; this is done in afterOperation()
            // after checking curls return code!
            f= download_total==0 ? 0 : 0.99f;
        }
        request->setProgress(f);
        return 0;
    }   // progressDownload



} // namespace Online
