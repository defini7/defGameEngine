function OnUserCreate()
	return true
end

function OnUserUpdate(delta_time)
	if GetMouse(0).pressed then
		DrawCircle(MousePos(), 10, Colors.WHITE)
	end

	return true
end

function CreateApp()
	app = {}

	app.dimensions = { 800, 600, 1, 1 }
	app.title = "Hello, Lua!"

	return app
end
