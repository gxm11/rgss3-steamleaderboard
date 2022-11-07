// author: guoxiaomi
// date: 2022/11/07
// website: https://rpg.blue/thread-491526-1-1.html

#define STEAM_API_EXPORTS
#include "steam/steam_api.h"
#include "windows.h"
#include <array>
#include <vector>

int test_index = 1000;
int leaderboard_id = 0;

struct leaderboard_item
{
    int rank;
    int score;
    std::array<int32_t, 32> detail;

    void clear()
    {
        score = 0;
        rank = 0;
        detail.fill(0);
    }
};

std::vector<leaderboard_item> leaderboard_data;

void wait(SteamAPICall_t ret)
{
    for (int i = 30; i != 0; --i) {
        Sleep(100);
        bool b = false;
        bool b2 = SteamUtils()->IsAPICallCompleted(ret, &b);
        if (b || b2) break;
    }
}

extern "C" void init(const char* name, int id)
{
    leaderboard_id = id;
    SteamAPICall_t ret = SteamUserStats()->FindLeaderboard(name);
    wait(ret);
}

extern "C" void upload_score(int score, const char* detail)
{
    if (!leaderboard_id) return;

    SteamAPICall_t ret = SteamUserStats()->UploadLeaderboardScore(
        leaderboard_id, k_ELeaderboardUploadScoreMethodForceUpdate, score,
        (int32_t*)detail, sizeof(leaderboard_item::detail) / sizeof(int32_t));
    wait(ret);
}

extern "C" void download_scores(int start, int end, int type)
{
    if (!leaderboard_id) return;

    SteamAPICall_t ret = SteamUserStats()->DownloadLeaderboardEntries(
        leaderboard_id, static_cast<ELeaderboardDataRequest>(type), start, end);
    wait(ret);

    while (test_index > 0)
    {
        LeaderboardEntry_t entry;

        SteamUserStats()->GetDownloadedLeaderboardEntry(
            test_index, 0, &entry, nullptr, 0);

        if (entry.m_steamIDUser.IsValid()) break;

        --test_index;
    }

    int size = end - start + 1;
    leaderboard_data.resize(size);

    for (int index = 0; index < size; ++index) {
        leaderboard_item& item = leaderboard_data.at(index);
        item.clear();

        LeaderboardEntry_t entry;

        SteamUserStats()->GetDownloadedLeaderboardEntry(
            test_index, index, &entry, item.detail.data(), item.detail.size());

        if (entry.m_steamIDUser.IsValid())
        {
            item.score = entry.m_nScore;
            item.rank = entry.m_nGlobalRank;
        }
    }

    test_index += 1000;
}

extern "C" void read_item(int index, const char* buffer)
{
    if (!leaderboard_id) return;
    if (index < 0 && index >= leaderboard_data.size()) return;

    leaderboard_item& item = leaderboard_data.at(index);
    memcpy((void*)buffer, &item, sizeof(leaderboard_item));
}

extern "C" void get_person_name(const char* buffer)
{
    const char* name = SteamFriends()->GetPersonaName();
    memcpy((void*)buffer, name, std::min<size_t>(256, strlen(name)));
}