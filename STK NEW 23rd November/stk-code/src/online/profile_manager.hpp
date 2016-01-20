//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 Glenn De Jonghe
//
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

#ifndef HEADER_ONLINE_PROFILE_MANAGER_HPP
#define HEADER_ONLINE_PROFILE_MANAGER_HPP

#include "utils/types.hpp"
#include "online/profile.hpp"


#include <irrString.h>

#include <string>

namespace Online{
    /**
      * \brief Class that takes care of online profiles
      * \ingroup online
      */
    class ProfileManager
    {

        private:
            ProfileManager      ();
            ~ProfileManager     ();

            typedef std::map<uint32_t, Profile*>    ProfilesMap;

            /** A map of profiles that is persistent. (i.e. no automatic removing happens) Normally used for the current user's profile and friends. */
            ProfilesMap                             m_profiles_persistent;
            /**
             * Any profiles that don't go into the persistent map, go here.
             * Using a Least Recent Used caching algorithm with age bits to remove entries when the max size is reached.
             **/
            ProfilesMap                             m_profiles_cache;
            Profile *                               m_currently_visiting;
            /** The max size of the m_profiles cache.  */
            static const unsigned int               m_max_cache_size = 20;

            void                                    iterateCache(Profile * profile);
            void                                    directToCache(Profile * profile);

        public:
            /**Singleton */
            static ProfileManager *                 get();
            static void                             deallocate();

            void                                    addToCache(Profile * profile);
            void                                    addPersistent(Profile * profile);
            void                                    deleteFromPersistent(const uint32_t id);
            void                                    clearPersistent();
            void                                    moveToCache(const uint32_t id);
            void                                    setVisiting(const uint32_t id);
            bool                                    cacheHit(const uint32_t id);
            bool                                    inPersistent(const uint32_t id);
            /** \return the instance of the profile that's currently being visited */
            Profile *                               getVisitingProfile() {return m_currently_visiting;}
            Profile *                               getProfileByID(const uint32_t id);

    };   // class CurrentUser

} // namespace Online

#endif

/*EOF*/
