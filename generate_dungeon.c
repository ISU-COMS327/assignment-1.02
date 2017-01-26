#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define HEIGHT 105
#define WIDTH 160
#define IMMUTABLE_ROCK -1
#define ROCK 0
#define ROOM 1
#define CORRIDOR 2
#define MIN_NUMBER_OF_ROOMS 10
#define MAX_NUMBER_OF_ROOMS 25
#define MIN_ROOM_WIDTH 7
#define DEFAULT_MAX_ROOM_WIDTH 15
#define MIN_ROOM_HEIGHT 5
#define DEFAULT_MAX_ROOM_HEIGHT 13 

int board[HEIGHT][WIDTH] = {{ROCK}};
int NUMBER_OF_ROOMS = MIN_NUMBER_OF_ROOMS;
int MAX_ROOM_WIDTH = DEFAULT_MAX_ROOM_WIDTH;
int MAX_ROOM_HEIGHT = DEFAULT_MAX_ROOM_HEIGHT;

struct Room {
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};

int random_int(int min_num, int max_num, int add_to_seed);
void initialize_immutable_rock();
void print_board();
void print_cell();
void dig_rooms(int number_of_rooms_to_dig);
void dig_room(int index, int recursive_iteration);
int room_is_valid_at_index(int index);
void add_rooms_to_board();
void dig_cooridors();
void connect_rooms_at_indexes(int index1, int index2);
struct Room rooms[MAX_NUMBER_OF_ROOMS];

int main(int argc, char *args[]) {
    // Parse arguments
    int opt;
    while((opt = getopt(argc, args, "hn:")) != -1) {
        switch(opt) {
            case 'n':
                NUMBER_OF_ROOMS = atoi(optarg);
                break;
            case 'h':
                printf("usage: generate_dungeon [-n <number of rooms>]\n\n");
                exit(0);
        }
    }
    
    printf("Generating dungeon... \n");

    // Determine # of rooms
    if (NUMBER_OF_ROOMS < MIN_NUMBER_OF_ROOMS) {
        printf("Minimum number of rooms is %d\n", MIN_NUMBER_OF_ROOMS);
        NUMBER_OF_ROOMS = MIN_NUMBER_OF_ROOMS;
    }
    if (NUMBER_OF_ROOMS > MAX_NUMBER_OF_ROOMS) {
        printf("Maximum number of rooms is %d\n", MAX_NUMBER_OF_ROOMS);
        NUMBER_OF_ROOMS = MAX_NUMBER_OF_ROOMS;
    }

    printf("Making %d rooms.\n", NUMBER_OF_ROOMS);

    // Generate board
    initialize_immutable_rock();
    dig_rooms(NUMBER_OF_ROOMS);
    dig_cooridors();
    print_board();
    return 0;
}

int random_int(int min_num, int max_num, int add_to_seed) {
    int seed = time(NULL);
    if (add_to_seed) {
        seed += add_to_seed * 10;
    }
    max_num ++;
    int delta = max_num - min_num;
    srand(seed);
    return (rand() % delta) + min_num;
}

void initialize_immutable_rock() {
    int y;
    int x;
    int max_x = WIDTH - 1;
    int max_y = HEIGHT - 1;
    for (y = 0; y < HEIGHT; y++) {
        board[y][0] = IMMUTABLE_ROCK;
        board[y][max_x] = IMMUTABLE_ROCK;
    }
    for (x = 0; x < WIDTH; x++) {
        board[0][x] = IMMUTABLE_ROCK;
        board[max_y][x] = IMMUTABLE_ROCK;
    }
}

void print_board() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            print_cell(board[y][x]);
        }
        printf("\n");
    }
}

void print_cell(int cell) {
    if (cell == ROCK || cell == IMMUTABLE_ROCK) {
        printf(" ");
    }
    else if (cell == ROOM) {
        printf(".");
    }
    else if (cell == CORRIDOR) {
        printf("#");
    }
    else {
        printf("F");
    }
}

void dig_rooms(int number_of_rooms_to_dig) {
    for (int i = 0; i < number_of_rooms_to_dig; i++) {
        dig_room(i, 0);
    }
    add_rooms_to_board();
}

void dig_room(int index, int recursive_iteration) {
    // The index + recusrive_iteration is just a way to gain variety in the 
    // random number. The hope is that it makes the program run faster.
    int start_x = random_int(0, WIDTH - MIN_ROOM_WIDTH - 1, index + recursive_iteration * 10);
    int start_y = random_int(0, HEIGHT - MIN_ROOM_HEIGHT - 1, index + recursive_iteration / 10);
    int room_height = random_int(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT, index + recursive_iteration - 5000);
    int room_width = random_int(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH, index + recursive_iteration + 5000);
    int end_y = start_y + room_height;
    if (end_y >= HEIGHT - 1) {
        end_y = HEIGHT - 2;
        
    }
    int end_x = start_x + room_width;
    if (end_x >= WIDTH - 1) {
        end_x = WIDTH - 2;
        
    }
    int height = end_y - start_y;
    int height_diff = MIN_ROOM_HEIGHT - height;
    if (height_diff > 0) {
        start_y -= height_diff + 1;
    }

    int width = end_x - start_x;
    int width_diff = MIN_ROOM_WIDTH - width;
    if (width_diff > 0) {
        start_x -= width_diff;
    }
    rooms[index].start_x = start_x;
    rooms[index].start_y = start_y;
    rooms[index].end_x = end_x;
    rooms[index].end_y = end_y;
    if (!room_is_valid_at_index(index)) {
        dig_room(index, recursive_iteration + 1);
    }
}

int room_is_valid_at_index(int index) {
    struct Room room = rooms[index];
    int width = room.end_x - room.start_x;
    int height = room.end_y - room.start_y;
    if (height < MIN_ROOM_HEIGHT || width < MIN_ROOM_WIDTH) {
        return 0;
    }
    for (int i = 0; i < index; i++) {
        struct Room current_room = rooms[i];
        int start_x = current_room.start_x - 1;
        int start_y = current_room.start_y - 1;
        int end_x = current_room.end_x + 1;
        int end_y = current_room.end_y + 1;
        if ((room.start_x >= start_x  && room.start_x <= end_x) ||
                (room.end_x >= start_x && room.end_x <= end_x)) {
            if ((room.start_y >= start_y && room.start_y <= end_y) ||
                    (room.end_y >= start_y && room.end_y <= end_y)) {
                return 0;
            }
        }
    }
    return 1;
}

void add_rooms_to_board() {
    for(int i = 0; i < NUMBER_OF_ROOMS; i++) {
        struct Room room = rooms[i];
        for (int y = room.start_y; y <= room.end_y; y++) {
            for(int x = room.start_x; x <= room.end_x; x++) {
                board[y][x] = ROOM;
            }
        }
    } 
}

void dig_cooridors() {
    for (int i = 0; i < NUMBER_OF_ROOMS; i++) {
        int next_index = i + 1;
        if (next_index == NUMBER_OF_ROOMS) {
            next_index = 0;
        }
        connect_rooms_at_indexes(i, next_index);
    }
}

void connect_rooms_at_indexes(int index1, int index2) {
    struct Room room1 = rooms[index1];
    struct Room room2 = rooms[index2];
    int start_x = ((room1.end_x - room1.start_x) / 2) + room1.start_x;
    int end_x = ((room2.end_x - room2.start_x) / 2) + room2.start_x;
    int start_y = ((room1.end_y - room1.start_y) / 2) + room1.start_y;
    int end_y = ((room2.end_y - room2.start_y) / 2) + room2.start_y;
    int x_incrementer = 1;
    int y_incrementer = 1;
    if (start_x > end_x) {
        x_incrementer = -1;
    }
    if (start_y > end_y) {
        y_incrementer = -1;
    }
    int cur_x = start_x;
    int cur_y = start_y;
    while(1) {
        int random_num = random_int(0, RAND_MAX, cur_x + cur_y) >> 3;
        int move_y = random_num % 2 == 0;
        if (board[cur_y][cur_x] != ROCK) {
            if (cur_y != end_y) {
                cur_y += y_incrementer;
            }
            else if(cur_x != end_x) {
                cur_x += x_incrementer;
            }
            else if(cur_y == end_y && cur_x == end_x) {
                break;
            }
            continue;
        }
        board[cur_y][cur_x] = CORRIDOR;
        if ((cur_y != end_y && move_y) || (cur_x == end_x)) {
            cur_y += y_incrementer;
        }
        else if ((cur_x != end_x && !move_y) || (cur_y == end_y)) {
            cur_x += x_incrementer;
        }
        else {
            break;
        }
    }
}
