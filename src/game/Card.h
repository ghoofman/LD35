#pragma once

#include "OPengine.h"

#include "Global.h"
#include "cards\CardType.h"
#include "CardDesc.h"

struct Card;
typedef struct Card Card;

struct Player;
struct Scene;

struct Card {
	OPtexture2D* tex2d;
	CardDesc Desc;

	void Init(CardDesc desc) {
		Desc = desc;

		tex2d = OPtexture2DCreate((OPtexture*)OPcmanLoadGet(desc.Texture));
		tex2d->Scale = OPvec2Create(0.35, 0.35);
		tex2d->Position = OPvec2Create(-0.75, -0.75);
	}

	void Render(Character* player, Scene* scene, f32 placement, ui8 activeCard, i8 dir) {

		tex2d->Position = OPvec2Create((-0.85 + placement) * dir, -0.85 + activeCard * 0.2);
		tex2d->Scale = OPvec2Create(0.25 + activeCard * 0.1, 0.25 + activeCard * 0.1);
		OPtexture2DRender(tex2d);

		if (!Desc.Available(player, scene)) {
			UNAVILABLECARD->Position = OPvec2Create((-0.85 + placement) * dir, -0.85 + activeCard * 0.2);
			UNAVILABLECARD->Scale = OPvec2Create(0.25 + activeCard * 0.1, 0.25 + activeCard * 0.1);
			OPtexture2DRender(UNAVILABLECARD);
		}
	}
};


inline i8 CardIsAvailable(Character* player, Scene* scene) {
	return 1;
}
