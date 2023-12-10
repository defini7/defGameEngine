function OnUserCreate()
	return true
end

function OnUserUpdate(delta_time)
	return true
end

function CreateApp()
	app = {}

	app.dimensions = { 256, 240, 4, 4 }
	app.title = "Demo"

	return app
end
