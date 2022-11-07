steam_api = steam_api.dll
cflags = -shared -std=c++11 -lstdc++ -fPIC -O2 -s
cflags += -static-libstdc++ -static-libgcc -static

leaderboard.dll : leaderboard.cpp
	@g++ -o $@ $(cflags) leaderboard.cpp $(steam_api)

leaderboard.zip : leaderboard.dll
	@7z a -tzip leaderboard.zip *