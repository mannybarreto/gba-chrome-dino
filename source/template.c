#include <string.h>
#include <tonc.h>
#include "character.h"

// With 1024 bytes at our disposal in the OAM, we have room for 128 OBJ_ATTRs and 32 OBJ_AFFINEs
OBJ_ATTR object_buffer[128];
OBJ_AFFINE *object_affine_buffer = (OBJ_AFFINE *)object_buffer;

void render_character()
{
	int x = 96, y = 32;
	u32 tile_id = 2, pal_bank = 0;

	// Dereference the first object in the buffer.
	OBJ_ATTR *character = &object_buffer[0];

	// Easy object initializer
	obj_set_attr(character,
				 ATTR0_SQUARE | ATTR0_4BPP,					   // Square sprites...
				 ATTR1_SIZE_32,				   // of size 32x32p...
				 ATTR2_BUILD(tile_id, pal_bank, 1) // from palbank 0, and tile index 0.
	);

	// Position sprite.
	obj_set_pos(character, x, y);

	// Wait for vblank.
	vid_vsync();

	oam_copy(oam_mem, object_buffer, 1); // Update the object buffer.
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
{
	// Setting up sprites:
	// Sprites are placed in block 4 (lower) and 5 (higher).
	// Place the glyphs of the 4bpp boxed character sprite into LOW obj memory (cbb == 4).
	memcpy(&tile_mem[4][0], characterTiles, characterTilesLen);
	memcpy(pal_obj_mem, characterPal, characterPalLen);

	// Hide all the sprites.
	oam_init(object_buffer, 128);

	// Enabling sprites in the display control.
	REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_2D; // Enables rendering of sprites & Object mapping mode.

	render_character();

	while (1)
		;

	return 0;
}
