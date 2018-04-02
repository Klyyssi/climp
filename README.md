# Command line mediaplayer

Play videos and other media in the command line in ASCII.

`> climp video.avi`

![Playing a video](example.gif)


```
> climp tux_linux.jpg
                                              '"oO".                                                
                                        .OOOOOOOOOOOOOOOOOO                                         
                                      OOOOOOOOOOOOOOOOOOoOOOOO                                      
                                    OOOOOOOOOOOOOOOOOOOo","oOOOO.                                   
                                   OOOOOOOOOOOOOOOOOOOOoooOOOOOOOO                                  
                                  OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                                 
                                  OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"                                
                                  OOOOOOOOOoOOOOOOOOo,,.oOOoOOOOOOOO                                
                                  OOO".'...OOOOOOO,''   '."OOOOOOOOO                                
                                  OOO  Ooo  OOOOO'  'Oo,O  "OOOOOOOO                                
                                  OOo OOO,O OOOOO,  OOOOoO  OOOOOOOO                                
                                  OOO  OOO,,....,.."OOOOO  'OOOOOOOO,                               
                                  "OOO',,.....'''''''''.."oOOOOOOOOOO                               
                                  .OO,,.....'''''''''''''.'.OOOOOOOOO                               
                                   O",.....''''''''''.,.....OOOOOOOOOO                              
                                   OOO""".'''''',o,,....,,'"OOOo,,,OOOO                             
                                  'OOO''.""......,,,,.''''''OOOOO,,oOOOO                            
                                  OOOO ''''."""",.'''''      'OOOOOOOOOOO                           
                                OOOO.   ''''''''''''          'OOOOOOOOOOOO                         
                              OOOOO       ''''''                OOOOOOOOOOOO                        
                            OOOOOO                               OOOOOOOOOOOOO                      
                          OOOOOOO'                               OOOOOOOOOOOOOOO                    
                        oOOOOOOO..''                   '''''''''' OOOOOOOOOOOOOOOO                  
                       oOOOOOOOO.'                           '''''  OOOO"OOOOOOOOOO,                
                       OOO,OOO,                                   '' OOOOOO"OOOOOOOO"               
                      OOO"OOO                                       ' OOOooOOOOOOOOOO,              
                     OOOoOOO                                           OOooOOO"OOOOOOO              
                    OOO"OOO                                             OOOOOOOOOOOOOOO             
                   OOO,OOO                                              OOOOOOOoOOOOOOOo            
                  OOOoOOO'                                              OOOOOOOOOOOOOOOO            
                OOOOOOOOO                                               OOOOOO,OOOOOOOOO            
               "OOOOo"oOo                                               OOOOOOOOOOOOOOOO            
               OOOOOOO"O"                                               OOOOOOOOOOOOOOOO            
                O,.'.,OO"                                               OoOOOOOOOOOo,OOO            
               ,..''''''OOO.                                       .'''',OOOOOOOOOOooO.             
             ',..''''''''.OOOO                                     .'''.OOOOOOOOOOOOO.'''           
     ,..........'''''''''''.OOOOO'                               ''.....,OOOOOOOOOO,....'           
    ,....'''''''''''''''''''.OOOOOOO                           ''''.....,,,,,,,,,,....'''.          
    ,...'''''''''''''''''''''.oOOOOOOO,                         ''',...............''''''...        
    '....''''''''''''''''''''''.OOOOOOO                         ''O,..............''''''''''..      
     ...'''''''''''''''''''''''''.OOO                          'OOo,...''''''''''''''''''''''''''.  
    ....''''''''''''''''''''''''''.,                         OOOOO",...''''''''''''''''''''''''..,  
    ,..'''''''''''''''''''''''''...,"'                   OOOOOOOOO",...'''''''''''''''''''''....    
   ,,...'''''''''''''''''''''''''...,"OOOOo.  '.,oOOOOOOOOOOOOOOOo",...'''''''''''''.''...,,        
   ,,,,..............'''''''''''...,,"oOOOOOOOOOOOOOOOOOOOOOOOOOOo",....''''''.......,,             
         ",,,,,,,,,,,.......'.....,"""oOOOOOOOOOOOOOOOOOOOOOOOOOOo",,,...........,,"                
                 .o""""",,,,,,,,,"""ooO                          Oo"",,,,,,,,,,""                   
                         OoooooooooO                               ooo"""""""oO
```

## Usage

### Play a video

`climp [OPTIONS]... [FILENAME]`

```
OPTIONS
  -c [CHARSET]
      Specify a custom set of characters.

  -w [WIDTH]
      Set width. Preserves aspect ratio if height is not set.

  -h [HEIGHT]
      Set height. Preserves aspect ratio if width is not set.
```

```
EXAMPLES
  climp video.mkv
  climp -w 100 -h 50 -c " .,xX" video.mkv
```

### Display an image

```
EXAMPLES
  cat image.jpg|climp
  cat image.jpg|climp -w 100 -h 50 -c " .,xX"
```

## Dependencies

| Library        | Version        | Details           |
| -------------- | -------------- | ----------------- |
| libncurses     | 6.0.20160213   | I assume that the program will compile against newer versions as well. |
| libavcodec     | 56/60/100      | MAJOR/MINOR/MICRO |
| libavformat    | 56/40/101      | MAJOR/MINOR/MICRO |
| libjpeg-turbo  |                | TODO: Enter working version number. I assume that the program will compile against any new version. |

## FFmpeg documentation

Use documentation for FFmpeg version 2.8.

http://www.ffmpeg.org/doxygen/2.8/

## Compilation

Generally, make sure you have installed all required dependencies, and then run `make`.

### Ubuntu 16.04

```
sudo apt install libjpeg-turbo8-dev
sudo apt install libavcodec-dev
sudo apt install libavformat-dev
sudo apt install libncurses5-dev
make
```

## TODO

- Colours
- Subtitles
- Audio
- Input devices
- Support for more image formats
