#include "my_mouse.h"
void get_chars(MazeConfig** parsed_config, char *config){
    (*parsed_config)->full = config[0];
    (*parsed_config)->empty = config[1];
    (*parsed_config)->path = config[2];
    (*parsed_config)->entrance = config[3];
    (*parsed_config)->exit = config[4];
}
char* get_config(char* buffer,char* config){
    int i = 0;
    while(buffer[i] != '\n'){
        if(i==14){
            return NULL;
        }
        config[i] = buffer[i];
        i++;
    }
    return &buffer[i+1];
}
int get_height(char** config){
    char height[5] = {'\0'};//no number allowed past 1000-a UI would be needed to guarantee this
    int i = 0;
    while(is_digit((*config)[i])){
        height[i] = (*config)[i];
        i++;
    }
    (*config)+=(i);//do not want to remove next character (the full character)
    return atoi(height);
}
int get_width(char** config){
    char width[5] = {'\0'};//no number allowed past 1000-a UI would be needed to guarantee this
    int i = 0;
    while(is_digit((*config)[i])){
        width[i] = (*config)[i];
        i++;
    }
    (*config)+=(i+1);//remove 'x'
    return atoi(width);
}
int init_config(MazeConfig *parsed_config, char* config, char* buffer, char** maze, int* n, int* m){
    if( (*maze = get_config(buffer,config)) == NULL){
        printf("Invalid Configuration.\n");
        return EXIT_FAILURE;
    }
    if( (parse_config(parsed_config,config) == -1) ){
        printf("Invalid Configuration.\n");
        return EXIT_FAILURE;
    }
    *n = parsed_config->height;
    *m = parsed_config->width;
    if(*n > MAX_HEIGHT || *m > MAX_WIDTH){
        printf("Matrix size specified by user exceeds limits.\n");
        return EXIT_FAILURE;
    }else if(*n != *m){
        printf("Maze must be a square.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
bool is_digit(char c){
    return (c>47 && c<58);
}
int open_file(char* filename, struct stat *sb, size_t* size, int* fd){
    *fd = open(filename,O_RDONLY);
    if(*fd == -1){
        handle_error("open");
        return EXIT_FAILURE;
    }
    if(fstat(*fd,sb) == -1){
        handle_error("fstat");
        return EXIT_FAILURE;
    }
    *size = sb->st_size;
    return EXIT_SUCCESS;
}
int parse_config(MazeConfig *parsed_config,char *config){
    if( (parsed_config->width = get_width(&config)) == 0) return -1;
    if( (parsed_config->height = get_height(&config)) ==0) return -1;
    get_chars(&parsed_config,config);//assumes config charatcers are correct
    return 0;
}
int read_file(char* buffer,size_t size, int fd){
    int i = read(fd,buffer,size);
    buffer[i] = '\0';
    close(fd);
    return i;
}