# rgss3-steamleaderboard
Using steam leaderboard in RMVA (RGSS3).

As a patch for the famous work https://github.com/GMMan/RGSS_SteamUserStatsLite, I add three methods for the class `SteamUserStatsLite`.

## usage
1. create a leaderboard in steamworks and keep the name and id of it.
2. import https://github.com/GMMan/RGSS_SteamUserStatsLite into your scripts.
3. import `leaderboard.rb` into your scripts.
4. add `leaderboard.dll` to your project.

## script
```ruby
# create the UserStatsLite instance
steam = SteamUserStatsLite.instance

# initialize the leaderboard by its name and id
steam.leaderboard_initialize("highscore", 9393678)

# upload score, the default detail is the user name.
steam.leaderboard_upload_score(100)

# download scores
steam.leaderboard_download_scores(1, 10).each { |rank, score, detail|
  p rank, score, detail
}
```

## compile
In MSYS2 Mingw32 env, just run `make` to compile the `leaderboard.dll`
