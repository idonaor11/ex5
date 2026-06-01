/******************
Name:ido naor
ID: 212499339
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;


typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

//read a string
char *readString();
//add a playlist
void addPlaylist(Playlist **playlist, int *currentAmount);
//add a song to a playlist
void addSong(Playlist **playlist, int choice, int *currentAmount);
//show playlist details
void showPlaylist(Playlist playlist[],int choice, int *currentAmount);
//print all playlists
void printPlaylist(Playlist playlist[], int *currentAmount);
//sort playlist by year
void sortByYear(Playlist playlist[],int choice);
//sort playlist by streams (descending)
void sortByDescending(Playlist playlist[],int choice);
//sort playlist by streams (ascending)
void sortByAscending(Playlist playlist[],int choice);
//sort playlist by song title
void sortBySongTitle(Playlist playlist[],int choice);
//sort playlist by different options
void sort (Playlist playlist[],int choice, int *currentAmount);
//choose a song to delete
int songsToDelete(Playlist playlist[],int choice);
//delete a song
void deleteSong(Playlist playlist[], int choice, int index);
//remove a playlist
void removePlaylist(Playlist **playlist, int *currentAmount, int index);
//free all memory at the end of the program
void freeAll(Playlist *playlist, int *currentAmount);
//select a playlist to delete
int playlistToDelete(Playlist playlist[], int *currentAmount);
//play a song
void playSong(Song *song);
//play all songs in a playlist
void playAll(Playlist playlist[], int choice);
//print the playlist menu
void printPlaylistsMenu();
//handle playlist options
void playlistMenu (Playlist playlist[],int choice, int *currentAmount);



int main() {
    int playlistCount = 0;
    Playlist *playlist = NULL; // initialize playlist
    playlist = (Playlist *)malloc(sizeof(Playlist)); // allocate memory for playlist
    if (playlist == NULL) {
        exit(1); // exit if memory allocation fails
    }
    int option;
    do {
        printPlaylistsMenu(); // print the menu
        scanf("%d", &option); // read user option
        switch (option) {
            case 1: {
                printPlaylist(playlist, &playlistCount); // show all playlists
                break;
            }
            case 2: {
                addPlaylist(&playlist, &playlistCount); // add new playlist
                break;
            }
            case 3: {
                int index = playlistToDelete(playlist, &playlistCount); // select playlist to delete
                removePlaylist(&playlist, &playlistCount, index); // delete the selected playlist
                break;
            }
            default:
        }
    } while(option != 4); // exit condition
    freeAll(playlist, &playlistCount); // free memory before exit
    printf("Goodbye!\n");
}

//read a string input from user
char *readString() {
    char *str = (char*) malloc(1); // allocate memory for string
    if (str == NULL) {
        exit(1); // exit if memory allocation fails
    }
    int size = 0;
    char c;
    scanf(" %c", &c); // read first character
    while (1) {
        if (c == '\n' || c == '\r') {
            break; // end of input when newline or carriage return is encountered
        }
        str = realloc(str, sizeof(char) * (size + 1)); // reallocate memory for string
        str[size++] = c; // add character to string
        scanf("%c", &c); // read next character
    }
    str[size] = '\0'; // terminate the string
    return str;
}

// add a new playlist
void addPlaylist(Playlist **playlist, int *currentAmount) {
    *playlist = realloc(*playlist, sizeof(Playlist) * (*currentAmount + 1)); //expand the playlist array
    int insertingIndex = *currentAmount;
    printf("Enter playlist's name:\n");
    char *playlistName = readString(); // read playlist name
    (*playlist)[insertingIndex].name = playlistName;
    (*playlist)[insertingIndex].songs = NULL; // initialize songs list
    (*playlist)[insertingIndex].songsNum = 0; //no songs yet
    *currentAmount = *currentAmount + 1; // increase the playlist counter
}

// add a new song to the playlist
void addSong(Playlist **playlist, int choice, int *currentAmount) {
    int insertingIndex = choice - 1; // playlist index
    int year = 0;
    Song *song = (Song *)malloc(sizeof(Song)); // allocate memory for the song
    if (song == NULL) {
        exit(1); // exit if memory allocation fails
    }
    printf("Enter song's details\n");
    printf("Title:\n");
    char *title = readString(); // read song title
    printf("Artist:\n");
    char *artist = readString(); // read song artist
    printf("Year of release:\n");
    scanf(" %d", &year); // read release year
    printf("Lyrics:\n");
    char *lyrics = readString(); // read song lyrics
    song->title = title;
    song->artist = artist;
    song->year = year;
    song->lyrics = lyrics;
    song->streams = 0; // initialize streams to 0

    // allocate memory for song list in the playlist
    (*playlist)[insertingIndex].songs = realloc((*playlist)[insertingIndex].songs,
    ((*playlist)[insertingIndex].songsNum + 1) * sizeof(Song *));
    if ((*playlist)[insertingIndex].songs == NULL) {
        exit(1); // exit if memory allocation fails
    }
    // add song to playlist
    (*playlist)[insertingIndex].songs[(*playlist)[insertingIndex].songsNum] = song;
    (*playlist)[insertingIndex].songsNum++; // increment song count

    playlistMenu(*playlist, choice, currentAmount); // return to the playlist menu
}


//show the details of the playlist
void showPlaylist(Playlist playlist[],int choice, int *currentAmount) {
    int counter = *currentAmount;
    int index = 0;
    for (int i = 0; i < playlist[choice - 1].songsNum; i++) {// loop through all songs in the chosen playlist
        printf("%d. Title: %s\nArtist: %s\nReleased: %d\nStreams: %d\n\n", i + 1,
               playlist[choice -1].songs[i]->title,
               playlist[choice -1].songs[i]->artist,
               playlist[choice -1].songs[i]->year,
               playlist[choice -1].songs[i]->streams); // print song details
    }
    printf("choose a song to play, or 0 to quit:\n");
    scanf(" %d",&index); // read song index or 0 to quit
    while (index != 0 || playlist[choice - 1].songsNum != 0) { // keep asking for a song until quit
        if (index > 0 && index <= playlist[choice - 1].songsNum) {
            playSong(playlist[choice - 1].songs[index - 1]); // play the chosen song
        }
        if (index > playlist[choice - 1].songsNum || index <= 0) {
            break;  //exit loop if invalid index
        }
        printf("choose a song to play, or 0 to quit:\n");
        scanf(" %d",&index);  //read next input
    }
    playlistMenu(playlist, choice, &counter);  //return to the playlist menu
}

//displays the list of playlists and lets the user choose one
void printPlaylist(Playlist playlist[], int *currentAmount) {
    int counter = *currentAmount;
    int option = 0;
    printf("Choose a playlist: \n");
    // print the playlists
    for (int i = 0; i < counter; i++) {
        printf("\t%d. %s \n", i + 1, playlist[i].name);
    }
    printf("\t%d. Back to main menu \n", counter + 1);
    scanf(" %d", &option);
    // invalid option handling
    if (option < 1 || option > counter + 1) {
        printf("Invalid option\n");
        printPlaylist(playlist, &counter); //recursive call if invalid
    }
    //return to main menu if selected
    if (option == counter + 1) {
        return;
    }
    // display selected playlist and show menu for actions
    printf("playlist %s:\n", playlist[option - 1].name);
    playlistMenu(playlist, option, &counter);
}

// sort the playlist by song release year
void sortByYear(Playlist playlist[], int choice) {
    Song *ptr = NULL;
    for (int i = 0; i < playlist[choice - 1].songsNum - 1; i++) {
        for (int j = i + 1; j < playlist[choice - 1].songsNum; j++) {
            if (playlist[choice - 1].songs[i]->year > playlist[choice - 1].songs[j]->year) {
                // swap songs
                ptr = playlist[choice - 1].songs[i];
                playlist[choice - 1].songs[i] = playlist[choice - 1].songs[j];
                playlist[choice - 1].songs[j] = ptr;
            }
        }
    }
}

// sort the playlist by streams in descending order
void sortByDescending(Playlist playlist[], int choice) {
    Song *ptr = NULL;
    for (int i = 0; i < playlist[choice - 1].songsNum - 1; i++) {
        for (int j = i + 1; j < playlist[choice - 1].songsNum; j++) {
            if (playlist[choice - 1].songs[i]->streams < playlist[choice - 1].songs[j]->streams) {
                // swap songs
                ptr = playlist[choice - 1].songs[i];
                playlist[choice - 1].songs[i] = playlist[choice - 1].songs[j];
                playlist[choice - 1].songs[j] = ptr;
            }
        }
    }
}

// sort the playlist by streams in ascending order
void sortByAscending(Playlist playlist[], int choice) {
    Song *ptr = NULL;
    // bubble sort by streams (ascending)
    for (int i = 0; i < playlist[choice - 1].songsNum - 1; i++) {
        for (int j = i + 1; j < playlist[choice - 1].songsNum; j++) {
            if (playlist[choice - 1].songs[i]->streams > playlist[choice - 1].songs[j]->streams) {
                // swap songs
                ptr = playlist[choice - 1].songs[i];
                playlist[choice - 1].songs[i] = playlist[choice - 1].songs[j];
                playlist[choice - 1].songs[j] = ptr;
            }
        }
    }
}

// sort the playlist alphabetically by song title
void sortBySongTitle(Playlist playlist[], int choice) {
    Song *ptr = NULL;
    for (int i = 0; i < playlist[choice - 1].songsNum - 1; i++) {
        for (int j = i + 1; j < playlist[choice - 1].songsNum; j++) {
            if (strcmp(playlist[choice - 1].songs[i]->title, playlist[choice - 1].songs[j]->title) > 0) {
                // swap songs
                ptr = playlist[choice - 1].songs[i];
                playlist[choice - 1].songs[i] = playlist[choice - 1].songs[j];
                playlist[choice - 1].songs[j] = ptr;
            }
        }
    }
}

//sort a playlist based on user's choice
void sort (Playlist playlist[],int choice, int *currentAmount) {
    int counter= *currentAmount;
    printf("choose:\n");
    printf("\t1. sort by year\n\t2. sort by streams - ascending order\n"
           "\t3. sort by streams - descending order\n\t4. sort alphabetically\n");
    int sortOption;
    scanf(" %d",&sortOption);
    //calling the relevant sort function
    switch (sortOption) {
        case 1: {
            sortByYear(playlist,choice);
            printf("sorted\n");
            playlistMenu(playlist,choice,&counter);
            break;
        }
        case 2: {
            sortByAscending(playlist,choice);
            printf("sorted\n");
            playlistMenu(playlist,choice,&counter);
            break;
        }
        case 3: {
            sortByDescending(playlist,choice);
            printf("sorted\n");
            playlistMenu(playlist,choice,&counter);
            break;
        }
        case 4: {
            sortBySongTitle(playlist,choice);
            printf("sorted\n");
            playlistMenu(playlist,choice,&counter);
            break;
        }
        default:sortBySongTitle(playlist,choice);
        printf("sorted\n");
        playlistMenu(playlist,choice,&counter);
    }
}

//select a song for deletion from a playlist
int songsToDelete(Playlist playlist[], int choice) {
    int index = 0;
    // display songs in playlist
    for (int i = 0; i < playlist[choice - 1].songsNum; i++) {
        printf("%d. Title: %s\nArtist: %s\nReleased: %d\nStreams: %d\n", i + 1,
               playlist[choice - 1].songs[i]->title,
               playlist[choice - 1].songs[i]->artist,
               playlist[choice - 1].songs[i]->year,
               playlist[choice - 1].songs[i]->streams);
    }
    printf("choose a song to delete, or 0 to quit:\n");
    scanf(" %d", &index);
    return index;
}

// delete a song from a playlist
void deleteSong(Playlist playlist[], int choice, int index) {
    if (index == 0) { // if user chose 0, exit the function
        return;
    }
    if (index < 1 || index > playlist[choice - 1].songsNum) { // invalid index check
        return;
    }

    // free song data (memory cleanup)
    free(playlist[choice - 1].songs[index - 1]->title);
    free(playlist[choice - 1].songs[index - 1]->artist);
    free(playlist[choice - 1].songs[index - 1]->lyrics);
    free(playlist[choice - 1].songs[index - 1]);

    // shift remaining songs to fill the gap
    for (int i = index - 1; i < playlist[choice - 1].songsNum - 1; i++) {
        playlist[choice - 1].songs[i] = playlist[choice - 1].songs[i + 1];
    }

    playlist[choice - 1].songsNum--; //decrease the song count

    //reallocate memory for songs in playlist if needed
    if (playlist[choice - 1].songsNum > 0) {
        playlist[choice - 1].songs = realloc(playlist[choice - 1].songs,
        playlist[choice - 1].songsNum * sizeof(Song *));
        if (playlist[choice - 1].songs == NULL) { // check if reallocation failed
            exit(1);
        }
    } else { // if no songs left, free the song array
        free(playlist[choice - 1].songs);
        playlist[choice - 1].songs = NULL;
    }

    printf("Song deleted successfully.\n");
}

// remove a playlist from the system
void removePlaylist(Playlist **playlist, int *currentAmount, int index) {
    int counter = *currentAmount;

    if (index == counter + 1) { // user chose to go back to the main menu
        return;
    }
    if (index < 1 || index > counter + 1) { // invalid playlist index
        printf("Invalid option\n");
        return;
    }

    // free song data for each song in the playlist
    for (int i = 0; i < (*playlist)[index - 1].songsNum; i++) {
        free((*playlist)[index - 1].songs[i]->title);
        free((*playlist)[index - 1].songs[i]->artist);
        free((*playlist)[index - 1].songs[i]->lyrics);
        free((*playlist)[index - 1].songs[i]);
    }

    // free playlist song array and name
    free((*playlist)[index - 1].songs);
    free((*playlist)[index - 1].name);

    // shift remaining playlists to fill the gap
    for (int i = index - 1; i < *currentAmount - 1; i++) {
        (*playlist)[i] = (*playlist)[i + 1];
    }

    (*currentAmount)--; //decrease the playlist count

    //reallocate memory for the playlists array
    *playlist = realloc(*playlist, *currentAmount * sizeof(Playlist));
    if (*playlist == NULL && *currentAmount > 0) { // check if reallocation failed
        exit(1);
    }

    printf("Playlist deleted.\n");
}

// free all the memory at the end of the program
void freeAll(Playlist *playlist, int *currentAmount) {

    for (int i = 0; i < *currentAmount; i++) {
// free the song details
        for (int j = 0; j < playlist[i].songsNum; j++) {
            free(playlist[i].songs[j]->title);
            free(playlist[i].songs[j]->artist);
            free(playlist[i].songs[j]->lyrics);
            free(playlist[i].songs[j]);
        }

        // free the song array for the playlist
        free(playlist[i].songs);
        // free the playlist's name
        free(playlist[i].name);
    }
    // free the playlist array
    free(playlist);
}

// select a playlist to delete
int playlistToDelete(Playlist playlist[], int *currentAmount) {
    int counter = *currentAmount;
    int option = 0;
    printf("Choose a playlist: \n");
    // display all playlists
    for (int i = 0; i < counter; i++) {
        printf("\t%d. %s \n", i + 1, playlist[i].name);
    }
    printf("\t%d. Back to main menu \n", counter + 1);
    scanf(" %d", &option);

    return option; // return the selected playlist index
}

// play a one song
void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);
    printf("$ %s $\n", song->lyrics);
    song->streams++; // increase stream count
}

// play all songs in a playlist
void playAll(Playlist playlist[], int choice) {
    if (playlist[choice - 1].songsNum == 0) { // check if the playlist is empty
        printf("Invalid option\n");
        return;
    }

    // play each song in the playlist
    for (int i = 0; i < playlist[choice - 1].songsNum; i++) {
        Song *song = playlist[choice - 1].songs[i];
        printf("Now playing %s:\n", song->title);
        printf("$ %s $\n", song->lyrics);
        song->streams++; //increase stream count
    }
}
//print the playlist menu
void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

// menu for actions within a specific playlist
void playlistMenu(Playlist playlist[], int choice, int *currentAmount) {
    int counter = *currentAmount;

    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
    int option = 0;
    scanf(" %d", &option);
    // go to function by user choice
    switch (option) {
        case 1: {
            showPlaylist(playlist, choice, &counter);
            break;
        }
        case 2: {
            addSong(&playlist, choice, &counter);
            break;
        }
        case 3: {
            int index = songsToDelete(playlist, choice);
            deleteSong(playlist, choice, index);
            playlistMenu(playlist, choice, &counter);
            break;
        }
        case 4: {
            sort(playlist, choice, &counter);
            break;
        }
        case 5: {
            playAll(playlist, choice);
            playlistMenu(playlist, choice, &counter);
            break;
        }
        case 6: {
            printPlaylist(playlist, &counter);
            break;
        }
        default: printf("Invalid option\n"); //handle invalid input
    }
}





