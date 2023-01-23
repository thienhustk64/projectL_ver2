#include  "GameMap.h"
#include <stdio.h>
#include <conio.h>

void GameMap::LoadMap( char *name){
    FILE* fp = NULL;
    fp = fopen( name, "rb");
    if( fp == NULL)
        return;

    game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;
    for ( int i = 0; i < MAX_MAP_Y; i++){
        for( int j = 0; j < MAX_MAP_X; j++){
            fscanf( fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            if( val > 0){
                if( j > game_map_.max_x_){
                    game_map_.max_x_ = j;
                }

                if( i > game_map_.max_y_){
                    game_map_.max_y_ = i;
                }
            }
        }
    }

    game_map_.max_x_ = (game_map_.max_x_ + 1)*TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1)*TILE_SIZE;

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;

    game_map_.file_name_ = name;
    fclose( fp);
}

void GameMap::LoadTiles( SDL_Renderer *screen){
    char file_img[20];
    FILE *fp = NULL;

    for( int i = 0; i < MAX_TILE; i++){
        sprintf( file_img, "data//map//%d.png", i);
        fp = fopen( file_img, "rb");
        if( fp == NULL){
            continue;
        }
        fclose( fp);
        tile_mat[i].LoadImg( file_img, screen);
    }
}