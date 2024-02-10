### By xXxLord_Auch?xXx ######
from ytmusicapi import YTMusic
# import vlc
from pytube import YouTube
import time


#Main class - allows automated interactions with YouTube Music
ytmusic = YTMusic()

#Search for something - only songs
search_results_songs = ytmusic.search("Leonely bastards", filter="songs")

for count, element in enumerate(search_results_songs):
    print(f"\n{count} ---")
    print(f"\tTitulo: {element['title']}\n\tArtista(s):")
    for artist in element['artists']:
        print(f"\t\t{artist['name']}")
    print(f"\tId: {element['videoId']}")

videoUrl = "https://www.youtube.com/watch?v="+search_results_songs[0]['videoId']

##print(videoUrl)
yt = YouTube(videoUrl)

stream_url = yt.streams.filter(only_audio=True)[0].url
#print(stream_url)

print("Streaming audio fetched...")

#Stream
Instance = vlc.Instance()
player = Instance.media_player_new()

Media = Instance.media_new(stream_url)
player.set_media(Media)
player.play()

time.sleep(10)
while player.is_playing():
    pass


print("Success...")

exit()