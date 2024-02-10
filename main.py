### By xXxLord_Auch?xXx ######
from ytmusicapi import YTMusic
# import vlc
from pytube import YouTube
import time


######### Get all results for given search ########

# Main class - allows automated interactions with YouTube Music
ytmusic = YTMusic()

# Search for something - only songs
search_results_songs = ytmusic.search("kalo uchis moonlight", filter="songs")

# List results
for count, element in enumerate(search_results_songs):
    print(f"\n{count} ---")
    print(f"\tTitulo: {element['title']}\n\tArtista(s):")
    for artist in element['artists']:
        print(f"\t\t{artist['name']}")
    print(f"\tId: {element['videoId']}")

# Youtube music video URL
videoUrl = "https://www.youtube.com/watch?v="+search_results_songs[0]['videoId']
print(videoUrl)


####### Get song's URL ########

# Create a YT class for the video
yt = YouTube(videoUrl) 
# Stract the audio URL
stream_url = yt.streams.filter(only_audio=True)[0].url # May we speed up this proccess??
print("Audio stream fetched...")
print(stream_url)

###### STREAMING ######
# (Playing media straight from the Internet without storing it locally is known as Streaming)

