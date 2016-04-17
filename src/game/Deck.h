#pragma once

#include "OPengine.h"

#include "Card.h"
#include "CardDesc.h"

struct Deck;
typedef struct Deck Deck;

struct Deck {
	// All of the cards in the deck
	Card* cards;
	Card* temp;
	ui32 cardCount;
	ui32 maxCards;
	ui32 cardInd;



	i32 activeCard;
	ui32 handInd;
	ui32 handCount;
	ui32 handMax;
	Card* hand;


	void Init(ui32 max) {
		maxCards = max;
		cards = (Card*)OPalloc(sizeof(Card) * max);
		temp = (Card*)OPalloc(sizeof(Card) * max);
		cardCount = 0;
		cardInd = 0;

		activeCard = -1;
		handCount = 0;
		handMax = 4;
		hand = (Card*)OPalloc(sizeof(Card) * handMax);
	}

	void AddCard(CardDesc desc) {
		if (cardCount >= maxCards) return;
		cards[cardCount++].Init(desc);
	}

	void Shuffle() {

		for (ui32 i = 0; i < cardCount; i++) {
			temp[i] = cards[i];
		}

		i32 available = cardCount;
		for (ui32 i = 0; i < cardCount; i++) {
			ui32 ind = OPrandom() * available;
			cards[i] = temp[ind];
			if (available > 0 && ind < available - 1) {
				// Swap pos of the last card with this card
				temp[ind] = temp[available - 1];
			}
			available--;
		}
	}

	Card* DrawCard() {

		if (handCount >= handMax || cardInd >= cardCount - 1) return NULL;

		ASSERT(handCount < handMax, "Already at max cards, can't add another");
		Card* result = &cards[cardInd++];
		hand[handCount++] = *result;
		if (activeCard < 0) activeCard = 0;

		return result;
	}



	void ActivateCard(Character* player, Scene* scene) {
		if (activeCard < 0) return;

		if (hand[activeCard].Desc.Activate(player, scene)) {
			for (ui32 i = activeCard; i < handCount - 1; i++) {
				hand[i] = hand[i + 1];
			}
			handCount--;
			if (handCount <= 0) {
				activeCard = -1;
			}
			if (activeCard >= handCount) {
				activeCard = handCount - 1;
			}
			OPfmodPlay(OPfmodLoad("Audio/Activated.wav"));
			//activeCard--;
			//cardCount--;
		}
		else {
			OPfmodPlay(OPfmodLoad("Audio/Error.wav"));
		}
	}

	void KillCard() {
		if (activeCard < 0) return;

		for (ui32 i = activeCard; i < handCount - 1; i++) {
			hand[i] = hand[i + 1];
		}
		handCount--;
		if (handCount <= 0) {
			activeCard = -1;
		}
		if (activeCard >= handCount) {
			activeCard = handCount - 1;
		}
		// OPfmodPlay(OPfmodLoad("Audio/Activated.wav"));
	}

	void NextCard() {
		if (handCount <= 0) return;
		activeCard = (activeCard + 1) % handCount;
	}

	void PrevCard() {
		if (handCount <= 0) return;
		activeCard--;
		if (activeCard < 0) {
			activeCard = handCount - 1;
			return;
		}
		activeCard = activeCard % handCount;
	}
};
