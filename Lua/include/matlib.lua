require("LuaPlot")
require("include.matrix")

matlib = {}

function matlib.init()
	return LuaWindow.init()
end

function matlib.uninit()
	return LuaWindow.uninit()
end

function matlib.plot(title,x,y)
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return false
	end
	LuaWindow.plot(x.m_data,y.m_data,title)
	return true
end

local function validate(x,y,z)
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return false
	end
	
	if not z.b_init or z.m_data == nil then
		return false
	end
	
	if #x.m_data * #y.m_data ~= #z.m_data then
		return  false
	end
	
	if #x.m_data == 0 then
		return false
	end
	
	return true
end

function matlib.surface(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.surface3d(x.m_data,y.m_data,z.m_data,title)
	return true
end


function matlib.cylinder(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.cylinder(x.m_data,y.m_data,z.m_data,title)
	return true
end

function matlib.kaxis3d(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.kaxis3d(x.m_data,y.m_data,z.m_data,title)
	return true
end


function matlib.sphere(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.sphere(x.m_data,y.m_data,z.m_data,title)
	return true
end

function matlib.smooth(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.smooth(x.m_data,y.m_data,z.m_data,title)
	return true
end

function matlib.grid(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.grid(x.m_data,y.m_data,z.m_data,title)
	return true
end

function matlib.kaxis2d(title,x,y,z)
	if not validate(x,y,z) then
		return false
	end
	LuaWindow.kaxis2d(x.m_data,y.m_data,z.m_data,title)
	return true
end

function matlib.plot_points(title,x,y)
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return false
	end
	LuaWindow.plot_points(x.m_data,y.m_data,title)
	return true
end

function matlib.rend_points(title,x,y,z)
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return false
	end
	if not z.b_init or z.m_data == nil then
		return false
	end
	LuaWindow.rend_points(x.m_data,y.m_data,z.m_data,title)
	return true
end