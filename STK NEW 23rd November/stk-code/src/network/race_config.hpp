#ifndef RACE_CONFIG_HPP
#define RACE_CONFIG_HPP

#include <string>
#include <vector>
#include "utils/types.hpp"

class TrackInfo
{
    public:
    TrackInfo() { laps = 0; reversed = false; }
    std::string track;
    bool reversed;
    uint8_t laps;
};
class TrackVote
{
    public:
    TrackVote();

    void voteTrack(std::string track);
    void voteReversed(bool reversed);
    void voteLaps(uint8_t laps);

    TrackInfo track_info;

    bool has_voted_track;
    bool has_voted_reversed;
    bool has_voted_laps;
};
class RaceVote
{
    public:
    RaceVote();

    void voteMajor(uint8_t major);
    void voteRaceCount(uint8_t count);
    void voteMinor(uint8_t minor);
    void voteTrack(std::string track, uint8_t track_number = 0);
    void voteReversed(bool reversed, uint8_t track_number = 0);
    void voteLaps(uint8_t laps, uint8_t track_number = 0);

    bool hasVotedMajor() const;
    bool hasVotedRacesCount() const;
    bool hasVotedMinor() const;
    bool hasVotedTrack(uint8_t track_number = 0) const;
    bool hasVotedReversed(uint8_t track_number = 0) const;
    bool hasVotedLaps(uint8_t track_number = 0) const;

    uint8_t getMajorVote() const;
    uint8_t getRacesCountVote() const;
    uint8_t getMinorVote() const;
    std::string getTrackVote(uint8_t track_number = 0) const;
    bool getReversedVote(uint8_t track_number = 0) const;
    uint8_t getLapsVote(uint8_t track_number = 0) const;

    private:
    uint8_t m_major_mode;
    uint8_t m_minor_mode;
    uint8_t m_races_count; //!< Stores the number of races that will be in a GP
    bool m_has_voted_major;
    bool m_has_voted_minor;
    bool m_has_voted_races_count;
    std::vector<TrackVote> m_tracks_vote;
};

class RaceConfig
{
    public:
    RaceConfig();

    void setPlayerCount(uint8_t count);
    void setPlayerMajorVote(uint8_t player_id, uint8_t major);
    void setPlayerRaceCountVote(uint8_t player_id, uint8_t count);
    void setPlayerMinorVote(uint8_t player_id, uint8_t minor);
    void setPlayerTrackVote(uint8_t player_id, std::string track, uint8_t track_nb = 0);
    void setPlayerReversedVote(uint8_t player_id, bool reversed, uint8_t track_nb = 0);
    void setPlayerLapsVote(uint8_t player_id, uint8_t lap_count, uint8_t track_nb = 0);

    void computeRaceMode();
    void computeNextTrack();

    const TrackInfo* getNextTrackInfo() const;
    bool getReverse() const;
    bool getLapCount() const;

    protected:
    std::vector<TrackInfo> m_tracks;
    int m_minor_mode;
    int m_major_mode;
    unsigned int m_races_count;

    std::vector<RaceVote> m_votes;
    uint8_t m_max_players;
};


#endif // RACE_CONFIG_HPP
