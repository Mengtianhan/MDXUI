require("include.fft")

--
-- 一元线性回归模型
-- 根据 输入和输出
-- x 输入
-- y 输出
-- 得到 w 和 b
-- f(x) = w*x + b
-- 使用最小二乘法找到w和b的值
--
function parse(x,y)
	if x:size() ~= y:size() then
		return
	end
	
	local x2 = x^2
	local s1 = x2:sum(true):cell(1,1)
	local val = x:aval()
	local s2 = x:sum(true):cell(1,1)
	s2 = s2^2
	s2 = s2/x:size()
	local m = s1 - s2
	local y1 = 0
	for i = 1,x:size() do
		y1 = y1 + y:cell(i,1)*(x:cell(i,1)-val)
	end
	
	local w = y1/m
	
	local b = 0
	
	for i=1,x:size() do
		b = b+y:cell(i,1)-w*x:cell(i,1)
	end
	
	b = b/x:size()
	return w,b
end


--
-- 多元线性回归
-- x 为矩阵
-- y 也为矩阵
--
function mult_parse(x,y)
	if x:rows() ~= y:rows() then
		return 
	end
	
	if y:cols() ~= 1 then
		return 
	end
	local w = linearity:solve(x,y)
	return w
end


--
-- 
-- 创建回归函数
--
function linear_fun(w,b)
	return function(d) return w*d + b end
end


--
-- 创建多元回归函数
-- 
function linear_fun2(w,b)
	return function(d) return (d*w):plus(b) end
end