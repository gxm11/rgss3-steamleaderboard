# author: guoxiaomi
# date: 2022/11/07
# website: https://rpg.blue/thread-491526-1-1.html
# usage:
# steam = SteamUserStatsLite.instance
# steam.leaderboard_initialize("highscore", 9393678)
# steam.leaderboard_upload_score(100)
# steam.leaderboard_download_scores(1, 10).each{|x| p x}

module SteamLeaderBoard
  module_function

  def leaderboard_api(function_name, params_type, return_type = 'v')
    Win32API.new('leaderboard', function_name, params_type, return_type)
  end

  Init = leaderboard_api('init', 'pi')
  UploadScore = leaderboard_api('upload_score', 'ip')
  DownloadScores = leaderboard_api('download_scores', 'iii')
  ReadItem = leaderboard_api('read_item', 'ip')
  GetPersonName = leaderboard_api('get_person_name', 'p')

  DataRequestGlobal = 0
  DataRequestGlobalAroundUser = 1
  DataRequestFriends = 2

  def init(name, id)
    Init.call(name, id)
  end

  def upload_score(score, detail)
    if detail.size > 127
      puts 'detail too long!'
      return
    end
    puts "upload: score = #{score}, #{detail}"

    UploadScore.call(score, detail + "\0")
  end

  def download_scores(start, size, type)
    DownloadScores.call(start, start + size - 1, type)

    size.times.to_a.collect { |i| read_item(i) }
  end

  def read_item(index)
    buffer = "\0" * 256
    ReadItem.call(index, buffer)
    # rank, score, detail
    buffer.unpack('llZ*')
  end

  def person_name
    buffer = "\0" * 256
    GetPersonName.call(buffer)
    buffer.unpack1('Z*')
  end
end

class SteamUserStatsLite
  def leaderboard_initialize(name, id)
    SteamLeaderBoard.init(name, id) if @initted
  end

  def leaderboard_upload_score(score, detail = nil)
    return unless @initted

    detail ||= SteamLeaderBoard.person_name
    SteamLeaderBoard.upload_score(score, detail)
  end

  def leaderboard_download_scores(start, size, type = nil)
    return [] unless @initted

    type ||= SteamLeaderBoard::DataRequestGlobal
    SteamLeaderBoard.download_scores(start, size, type)
  end
end
