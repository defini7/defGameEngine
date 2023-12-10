function AddVectorOperations(t)
	function t.__add(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x + rhs, lhs.y + rhs)
		end

		return t:new(lhs.x + rhs.x, lhs.y + rhs.y)
	end

	function t.__sub(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x - rhs, lhs.y - rhs)
		end

		return t:new(lhs.x - rhs.x, lhs.y - rhs.y)
	end

	function t.__mul(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x * rhs, lhs.y * rhs)
		end

		return t:new(lhs.x * rhs.x, lhs.y * rhs.y)
	end

	function t.__div(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x / rhs, lhs.y / rhs)
		end

		return t:new(lhs.x / rhs.x, lhs.y / rhs.y)
	end

	function t.__idiv(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x // rhs, lhs.y // rhs)
		end

		return t:new(lhs.x // rhs.x, lhs.y // rhs.y)
	end

	function t.__mod(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x % rhs, lhs.y % rhs)
		end

		return t:new(lhs.x % rhs.x, lhs.y % rhs.y)
	end

	function t.__pow(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x ^ rhs, lhs.y ^ rhs)
		end

		return t:new(lhs.x ^ rhs.x, lhs.y ^ rhs.y)
	end

	function t.__unm(lhs)
		return vi2d:new(-lhs.x, -lhs.y)
	end

	function t.__tostring(lhs)
		return lhs:str()
	end

	function t.__eq(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x == rhs and lhs.y == rhs
		end

		return lhs.x == rhs.x and lhs.y == rhs.y
	end

	function t.__lt(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x < rhs and lhs.y < rhs
		end

		return lhs.x < rhs.x and lhs.y < rhs.y
	end

	function t.__le(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x <= rhs and lhs.y <= rhs
		end

		return lhs.x <= rhs.x and lhs.y <= rhs.y
	end
end

AddVectorOperations(vi2d)
AddVectorOperations(vf2d)
