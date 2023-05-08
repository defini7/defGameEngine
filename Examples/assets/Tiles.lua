local TILE = { EMPTY=0, COIN=1, GRASS=2, DIRT=3 }

local lookup =
{
	['.']=TILE.EMPTY,
	['#']=TILE.DIRT,
	['x']=TILE.GRASS,
	['o']=TILE.COIN
}

TILE_SIZE = {x=32, y=32}

SPRITES_FILE = "mario.png"
BACKGROUND_FILE = "sky.png"

function LoadLevel(host, level)

	map = ""
	size = {}

	if level == 1 then

		map =
		"........xxxxxxxx"..
		".....ooo........"..
		"................"..
		"..xxxxxxxx...xxx"..
		"..########...###"..
		"..########..x###"..
		"................"..
		"xxxxxxxxxxxxxxxx"

		size = {x=16, y=8}

	end

	CreateLevel(host, size.x, size.y)

	for y = 1, size.y do
		for x = 1, size.x do

			i = (y - 1) * size.x + x
			c = string.sub(map, i, i)

			SetTile(host, x-1, y-1, lookup[c])

		end
	end

end

-- returns file coordinates of sprite
function LoadSprite(host, tileId)

	if		tileId == TILE.COIN		then return {x=4*TILE_SIZE.x,y=0}
	elseif  tileId == TILE.GRASS	then return {x=5*TILE_SIZE.x,y=0}
	elseif  tileId == TILE.DIRT		then return {x=6*TILE_SIZE.x,y=0}
	end

end
