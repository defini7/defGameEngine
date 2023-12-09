stars = {}

function RandFloat(min, max)
	return min + math.random() * (max - min)
end

function AddStar(i)
	local lum = RandFloat(0.5, 1.0)

	stars[i] = {}
	stars[i].angle = RandFloat(0.0, 2.0 * math.pi)
	stars[i].speed = RandFloat(10, 100)
	stars[i].distance = RandFloat(bound / 10.0 * 0.5, bound * 0.5)
	stars[i].col = PixelF(lum, lum, lum, 1.0)
end

function OnUserCreate()
	origin = vf2d:new(ScreenWidth() / 2, ScreenHeight() / 2)
	bound = math.max(ScreenWidth(), ScreenHeight()) * 0.8

	for i=1, 200 do
		AddStar(i)
	end

	return true
end

function OnUserUpdate(delta_time)
	Clear(Colors.BLACK)

	for i=1, #stars do
		stars[i].distance = stars[i].distance + stars[i].speed * delta_time * (stars[i].distance / (bound * 0.5))

		if stars[i].distance > bound then
			AddStar(i)
		end

		local pos = vf2d:new(origin.x + math.cos(stars[i].angle) * stars[i].distance, origin.y + math.sin(stars[i].angle) * stars[i].distance)

		local col = stars[i].col
		local factor = stars[i].distance / bound * 0.8

		Draw(vi2d:new(math.floor(pos.x), math.floor(pos.y)), Pixel:new(math.floor(col.r * factor), math.floor(col.g * factor), math.floor(col.b * factor), 255))
	end

	return true
end

function CreateApp()
	app = {}

	app.dimensions = { 256, 240, 4, 4 }
	app.title = "Star Field"

	return app
end
