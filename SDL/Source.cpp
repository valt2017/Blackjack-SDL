#include <iostream>
#include <fstream>
#include <SDL.h>
#include <map>
#include "karty.h"

// @valt

//figures coordinates
const int BoardOffset = 2;
std::map<std::string, SDL_Rect> Cards;
const int CardSizeX = 81;
const int CardSizeY = 117;
std::map<const char, SDL_Texture *> Textures;
std::map<const char, std::string> Bitmaps;
const int posX = 100, posY = 100;
const int sizeX = 454, sizeY = 454;

int main(int argc, char ** argv) {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	// Error checks
	std::cout << "SDL_Init\n";
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Blackjack - left button for next card, right for stop", posX, posY, sizeX, sizeY, 0);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow error\n";
	}
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "SDL_CreateRenderer error\n";
	}
	// Load bitmaps
	Bitmaps[0] = "img/cards.bmp";
	// Create textures from bitmaps
	for (auto bitmap : Bitmaps) {
		SDL_Surface * bitmapSurface = SDL_LoadBMP(bitmap.second.c_str());
		// create texture 
		Textures[bitmap.first] = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
		if (Textures[bitmap.first] == NULL) {
			std::cout << bitmap.second.c_str() <<" SDL_CreateTextureFromSurface error\n";
		}
		else
			std::cout << bitmap.second.c_str() << " SDL_CreateTextureFromSurface OK\n";
		SDL_FreeSurface(bitmapSurface);
	}
	// Coordinates for cards from bitmap
	for (int j = 0; j < oko::poc_farieb; j++)
		for (int i = 0; i < oko::poc_hodnot; i++){
			std::string Card = "   ";
			Card[0] = char(oko::farby[j]);
			Card[1] = char(oko::hodnoty[i]);
			Cards[Card] = SDL_Rect{ i*CardSizeX, j*CardSizeY, CardSizeX, CardSizeY};
		}

	oko::balicek blackjack;
	oko::hrac player1("Jozef"), player2("Boss");
	std::cout << "click left button for next card, right for stop\n";

	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
			/* Mouse button down */
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Point mousePos;
				mousePos.x = e.motion.x;
				mousePos.y = e.motion.y;
				// Next card
				if (e.button.button == SDL_BUTTON_LEFT){
					// new game ?
					if (player2.VratSkore() != 0) {
						player1 = oko::hrac("Jozef");
						player2 = oko::hrac("Boss");
						blackjack = oko::balicek();
					}
					else {
						if ((player1.VratSkore() < 21) && (!blackjack.Prazdny()))
							player1.PrijataKarta(blackjack.DalsiaKarta());
						std::cout << player1.VypisHraca();
					}
				}
				// Oponent play
				if (e.button.button == SDL_BUTTON_RIGHT) {
					if (player1.VratSkore() != 0) {
						while ((player2.VratSkore() < 15) && (!blackjack.Prazdny()))
							player2.PrijataKarta(blackjack.DalsiaKarta());
						std::cout << player2.VypisHraca();
						std::cout << "Winner is:\n";
						std::cout << (player1 < player2).VypisHraca();
					}
				}
			}
			/* Mouse button up */
			if (e.type == SDL_MOUSEBUTTONUP) {
				SDL_Point mousePos;
				mousePos.x = e.motion.x;
				mousePos.y = e.motion.y;
				if (e.button.button == SDL_BUTTON_LEFT) {
				}
			}
		}
		SDL_RenderClear(renderer);
		// Select the color for drawing. It is set to red here.
		//SDL_SetRenderDrawColor(renderer, 0, 172, 44, 255);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// cards texture
		auto texture = Textures.find(0);
		if (texture != Textures.end()) {
			// 1.player
			for (int i = 0; i < player1.karty.size(); i++) {
				auto it = Cards.find(player1.karty[i].VypisKartu());
					if (it != Cards.end()){
						SDL_Rect DestR;
						DestR.x = i*CardSizeX;
						DestR.y = 0;
						DestR.h = CardSizeY;
						DestR.w = CardSizeX;
						SDL_RenderCopy(renderer, texture->second, &it->second, &DestR);
					}
				}
			// 2.player
			for (int i = 0; i < player2.karty.size(); i++) {
				auto it = Cards.find(player2.karty[i].VypisKartu());
				if (it != Cards.end()) {
					SDL_Rect DestR;
					DestR.x = i*CardSizeX;
					DestR.y = CardSizeY*2;
					DestR.h = CardSizeY;
					DestR.w = CardSizeX;
					SDL_RenderCopy(renderer, texture->second, &it->second, &DestR);
				}
			}
		}
		SDL_RenderPresent(renderer);
	}
	for (auto texture : Textures) {
		SDL_DestroyTexture(texture.second);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	std::cout << "SDL_Quit\n";
	return 0;
}