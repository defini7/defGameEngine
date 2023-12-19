function CalcRandomPos()
	return vi2d:new(math.random(0, world_size.x - 1), math.random(0, world_size.y - 1))
end

function NewGame()
	snake = { { color=Colors.DARK_GREEN, pos=CalcRandomPos() } }
	apple = CalcRandomPos()

	snake_dir = vi2d:new(1, 0)

	counter = 0.0
	delay = 0.15

	score = 0
	is_dead = false
end

function OnUserCreate()
	math.randomseed(os.time())

	tile_size = vi2d:new(8, 8)
	world_size = ScreenSize() // tile_size

	NewGame()

	return true
end

function OnUserUpdate(dt)
	if is_dead then
		if GetKey(Keys.SPACE).pressed then NewGame() end

		DrawString(vi2d:new(1, ScreenHeight() // 2), "Game over! Press SPACE to play again.", Colors.YELLOW)

		return true
	end

	if GetKey(Keys.LEFT).pressed then snake_dir = vi2d:new(-1, 0) end
	if GetKey(Keys.RIGHT).pressed then snake_dir = vi2d:new(1, 0) end
	if GetKey(Keys.UP).pressed then snake_dir = vi2d:new(0, -1) end
	if GetKey(Keys.DOWN).pressed then snake_dir = vi2d:new(0, 1) end

	if counter > delay then
		counter = 0.0

		table.insert(snake, 1, { color = RandomPixel(false), pos = snake[1].pos + snake_dir })

		if snake[1].pos == apple then
			score = score + 1

			if score % 5 == 0 then
				delay = delay - 0.001
			end

			apple = CalcRandomPos()
			snake[#snake + 1] = snake[#snake]
		end

		for i=2, #snake do
			snake[i].color = RandomPixel(false)

			if snake[i].pos == snake[1].pos then
				is_dead = true
			end
		end

		if snake[1].pos.x < 0 or snake[1].pos.y < 0 or snake[1].pos.x >= world_size.x or snake[1].pos.y >= world_size.y then
			is_dead = true
		end

		table.remove(snake, #snake)
	end

	counter = counter + dt

	Clear(Pixel:new(32, 32, 32, 255))

	DrawString(vi2d:new(2, 2), "Score: " .. score, Colors.YELLOW)

	for k, v in pairs(snake) do
		FillRectangle(tile_size * v.pos, tile_size, v.color)
	end

	FillRectangle(tile_size * apple, tile_size, Colors.RED)

	return true
end

function CreateApp()
	return
	{
		size = { 256, 240, 4, 4 },
		title = "Snake",
		full_screen = true
	}
end
