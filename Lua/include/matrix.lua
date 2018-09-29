require("LuaMatrix")

--[[matrix 实现矩阵的常用操作方法]]
matrix = {}

--[[linearity 实现一些线性计算,包括快速傅里叶变换,卷积,矩阵方程求解,多项式拟合]]
linearity = {}

-------------------------------------------------------------------------------------------------
--[[--------------------------------定义一个矩阵-------------------------------------------]]
-------------------------------------------------------------------------------------------------

--
-- 构造函数
-- 创建一个新的矩阵
-- 所有矩阵都得从该接口生成
--
function matrix:new()
	local tempm = {}
	setmetatable(tempm,self)
	self.__index = self
	return tempm
end

--
-- 下面所有的接口都必须先调用new生成矩阵后才能够调用下面的函数
-- 比如 m = matrix:new()
-- m:randu(4,4)
-- m:print()
-- 克隆一个矩阵
-- 新建数据
--
function matrix:clone()
	local m = matrix:new()
	
	if not self.b_init then
		return m
	end
	
	m.n_cols = self.n_cols
	m.n_rows = self.n_rows
	
	if self.m_data ~= nil then
		m.m_data = {}
		
		for i=1,#self.m_data do
			m.m_data[i] = self.m_data[i]
		end
		m.b_init = true
		
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		m.m_data = nil
		m.m_real = matrix:new()
		m.m_imag = matrix:new()
		m.m_real.n_rows = m.n_rows
		m.m_real.n_cols = m.n_cols
		m.m_real.b_init = true
		m.m_real.m_data = {}
		
		m.m_imag.n_rows = m.n_rows
		m.m_imag.n_cols = m.n_cols
		m.m_imag.m_data = {}
		m.m_imag.b_init = true
		
		for i=1,#self.m_real.m_data do
			m.m_real.m_data[i] = self.m_real.m_data[i]
			m.m_imag.m_data[i] = self.m_imag.m_data[i]
		end
		m.b_init = true
	end
	
	return m
end


--
-- 创建一个0矩阵
-- 参数 row 为矩阵的行数
-- 参数 col 为矩阵的列数
--
function matrix:zero(row,col)
	if row == nil or col == nil then
		if not self.b_init then
			return
		end
		
		if self.m_data ~= nil then
			for i=1,#self.m_data do
				self.m_data[i] = 0
			end
		elseif self.m_real ~= nil  and self.m_imag ~= nil then
			for i=1,#self.m_real.m_data do
				self.m_real.m_data[i] = 0
				self.m_imag.m_data[i] = 0
			end
		end
	else
		self.n_rows = row
		self.n_cols = col
		self.b_init = true
		self.m_data = {}
		for i=1,row*col do
			self.m_data[i] = 0
		end
	end
end


--
-- 创建一个1矩阵
-- 参数 row 为矩阵的行数
-- 参数 col 为矩阵的列数
--
function matrix:ones(row,col)
	if row == nil or col == nil then
		if not self.b_init then
			return
		end
		
		if self.m_data ~= nil then
			for i=1,#self.m_data do
				self.m_data[i] = 1
			end
		elseif self.m_real ~= nil  and self.m_imag ~= nil then
			for i=1,#self.m_real.m_data do
				self.m_real.m_data[i] = 1
				self.m_imag.m_data[i] = 1
			end
		end
	else
		self.n_rows = row
		self.n_cols = col
		self.b_init = true
		self.m_data = {}
		for i=1,row*col do
			self.m_data[i] = 1
		end
	end
end

--
-- 创建一个均匀分布随机数矩阵
-- 参数 row 为矩阵的行数
-- 参数 col 为矩阵的列数
--
function matrix:randu(row,col)
	if row == nil or col == nil then
		if not self.b_init then
			return
		end
		
		if self.m_data ~= nil then
			for i=1,#self.m_data do
				self.m_data[i] = math.random()
			end
		elseif self.m_real ~= nil  and self.m_imag ~= nil then
			for i=1,#self.m_real.m_data do
				self.m_real.m_data[i] = math.random()
				self.m_imag.m_data[i] = math.random()
			end
		end
	else
		self.n_rows = row
		self.n_cols = col
		self.b_init = true
		self.m_data = LuaMatrix.randu(row,col)
	end
end


--
-- 创建一个正太分布随机数矩阵
-- 参数 row 为矩阵的行数
-- 参数 col 为矩阵的列数
--
function matrix:randn(row,col)
	if row == nil or col == nil then
		if not self.b_init then
			return
		end
		
		if self.m_data ~= nil then
			for i=1,#self.m_data do
				self.m_data[i] = math.random()
			end
		elseif self.m_real ~= nil  and self.m_imag ~= nil then
			for i=1,#self.m_real.m_data do
				self.m_real.m_data[i] = math.random()
				self.m_imag.m_data[i] = math.random()
			end
		end
	else
		self.n_rows = row
		self.n_cols = col
		self.b_init = true
		self.m_data = LuaMatrix.randn(row,col)
	end
end


--
-- 创建单位矩阵
-- 参数 row 为矩阵的行数
-- 参数 col 为矩阵的列数
--
function matrix:identify(row,col)
	if row == nil or col == nil then
		--
		-- 对于现用矩阵那么重新修改数据
		--
		if not self.b_init then
			return
		end
		if self.m_data ~= nil then
			for i=1,self.n_rows do
				for j=1,self.n_cols do
					if i ~= j then
						self:set(i,j,0)
					else
						self:set(i,j,1)
					end
				end
			end
		elseif self.m_real ~= nil  and self.m_imag ~= nil then
			for i=1,self.n_rows do
				for j=1,self.n_cols do
					if i ~= j then
						self:set(i,j,0,0)
					else
						self:set(i,j,1,0)
					end
				end
			end
		end
	else
		--
		-- 对于新矩阵那么创建矩阵
		--
		self.n_rows = row
		self.n_cols = col
		self.b_init = true
		self.m_data = {}
		for i=1,row do
			for j=1,col do
				if i ~= j then
					self:set(i,j,0)
				else
					self:set(i,j,1)
				end
			end
		end
	end
end	

--
-- 填充一个复数矩阵
-- 将所有单元填充为指定值
-- real 为要填充的实部
-- imag 为要填充的虚部
--
function matrix:fill(real,imag)
	if not self.b_init then
		return false
	end
	
	if imag == nil then
		for i=1,self.n_cols*self.n_rows do
			self.m_data[i] = val
		end
		return true
	end
	
	if self.m_real == nil or self.m_imag == nil then
		return false
	end
	
	for i=1,self.n_cols*self.n_rows do
		self.m_real.m_data[i] = real
		self.m_imag.m_data[i] = imag
	end
	
	return true
end


--
-- 改变矩阵大小
-- 超出原大小部分以0填充
--
function matrix:resize(row,col)
	--
	-- 保存当前的数据
	-- 修改大小后从旧数据中copy数据
	--
	local temp = self:clone()
	self.n_rows = row
	self.n_cols = col
	self.b_init = true
	
	if self.m_data ~= nil then
		self.m_data = {}
		for i=1,row*col do
			self.m_data[i] = 0
		end
		
		for i=1,temp.n_rows do
			if i > row then
				break
			end
			
			for j=1,temp.n_cols do
				if j > col then
					break
				end
				
				self:cell(i,j,temp:cell(i,j))
			end
		end
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		self.m_real.n_rows = row
		self.m_real.n_cols = col
		self.m_real.b_init = true
		self.m_real.m_data = {}
		
		self.m_imag.n_rows = row
		self.m_imag.n_cols = col
		self.m_imag.b_init = true
		self.m_imag.m_data = {}
		
		for i=1,row*col do
			self.m_real.m_data[i] = 0
			self.m_imag.m_data[i] = 0
		end
		
		for i=1,temp.n_rows do
			if i > row then
				break
			end
			
			for j=1,temp.n_cols do
				if j > col then
					break
				end
				
				self:cell(i,j,temp:cell(i,j))
			end
		end
	else
		--
		-- 如果本身就没有数据
		-- 那么数据全为0
		--
		self.m_data = {}
		for i=1,row*col do
			self.m_data[i] = 0
		end
	end
end




-- 
-- 复数矩阵
-- real 为实数矩阵
-- imag 为虚部矩阵
-- real  和 imag 的大小必须相等
--
function matrix:cx(real,imag)
	if real.m_data == nil or imag.m_data == nil then
		return
	end
	
	if real.n_cols ~= imag.n_cols or real.n_rows ~= imag.n_rows then
		return
	end
	
	if real.n_cols == 0 or real.n_rows == 0 then
		return
	end
	
	if self.m_data ~= nil then
		self.m_data = nil
	end

	self.m_real = matrix:new()
	self.m_imag = matrix:new()
	
	self.m_real.b_init = true
	self.m_real.n_cols = real.n_cols
	self.m_real.n_rows = real.n_rows
	self.m_real.m_data = {}
	for i=1,#real.m_data do
		self.m_real.m_data[i] = real.m_data[i]
	end
	
	self.m_imag.b_init = true
	self.m_imag.n_cols = real.n_cols
	self.m_imag.n_rows = real.n_rows
	self.m_imag.m_data = {}
	for i=1,#imag.m_data do
		self.m_imag.m_data[i] = imag.m_data[i]
	end
	
	self.b_init = true
	self.n_cols = real.n_cols
	self.n_rows = real.n_rows
end

--
-- 针对复矩阵设置实部
--
function matrix:set_real(real)
	if real.m_data == nil or not real.b_init then
		return false
	end
	
	if self.m_real == nil or not self.b_init then
		return false
	end
	
	if self.n_rows ~= real.n_rows or self.n_cols ~= real.n_cols then
		return false
	end
	
	self.m_real = real:clone()
	return true
end


--
-- 针对复矩阵设置虚部
--
function matrix:set_imag(imag)
	if imag.m_data == nil or not imag.b_init then
		return false
	end
	
	if self.m_imag == nil or not self.b_init then
		return false
	end
	
	if self.n_rows ~= imag.n_rows or self.n_cols ~= imag.n_cols then
		return false
	end
	
	self.m_imag = imag:clone()
	return true
end

--
-- 切片
-- beg 开始位置
-- step 步进  step 小于 0 反向切
-- stop 结束
-- dim == 0 列 
-- dim == 1 行
-- 
function matrix:slice(step,dim,beg,stop)
	local m = matrix:new()
	if not self.b_init then
		return m
	end
	
	if dim == nil or dim <= 0 then
		dim = 0
	end
	
	if dim > 0 then
		dim = 1
	end
	
	--
	-- 如果不指定起始和结束位置
	-- 那么根据步进和维度进行推断
	--
	if beg == nil then
		if dim == 0 then
			if step > 0 then 
				beg = 1
			else
				beg = self.n_rows
			end
		else
			if step > 0 then 
				beg = 1
			else
				beg = self.n_cols
			end
		end
	end
	
	if stop == nil then
		if dim == 0 then
			if step > 0 then
				stop = self.n_rows
			else
				stop = 1
			end
		else
			if step > 0 then
				stop = self.n_cols
			else
				stop = 1
			end
		end
	end
		
	if dim == 0 then
		local row = 0
		m.n_cols = self.n_cols
		for i=beg,stop,step do
			row = row + 1
		end
		m.n_rows = row
		m.b_init = true
		local iscx = false
		if self.m_data ~= nil then
			m.m_data = {}
		elseif self.m_real ~= nil and self.m_imag ~= nil then
			m.m_real = zeros(m.n_rows,m.n_cols)
			m.m_imag = zeros(m.n_rows,m.n_cols)	
			iscx = true
		end
		
		row = 1
		for i=beg,stop,step do
			for j=1,self.n_cols do
				m:cell(row,j,self:cell(i,j))
			end
			row = row + 1
		end
	else
		m.n_rows = self.n_rows
		local col = 0
		for i=beg,stop,step do
			col = col + 1
		end
		m.n_cols = col
		local iscx = false
		m.b_init = true
		if self.m_data ~= nil then
			m.m_data = {}
		elseif self.m_real ~= nil and self.m_imag ~= nil then
			m.m_real = zeros(m.n_rows,m.n_cols)
			m.m_imag = zeros(m.n_rows,m.n_cols)
			iscx = true
		end
		
		for i=1,self.n_rows do
			col = 1
			for j=beg,stop,step do
				m:cell(i,col,self:cell(i,j))
				col = col + 1
			end
		end
		
	end
	
	return m
end


--
-- 将数组放在矩阵中进行管理
--
function matrix:attach(t,row,col,iscolmajor)
	if type(t) ~= "table" then
		return false
	end
	
	self.b_init = true
	self.n_rows = row
	self.n_cols = col
	self.m_data = {}
	if iscolmajor == nil  or iscolmajor then
		for i=1,row*col do
			self.m_data[i] = t[i]
		end
	else
		local m = array(t,col,row):t()
		self.m_data = m.m_data
	end
	return true
end



--
-- 获取矩阵中的原始数据
-- iscolmajor == true 以列为主序的矩阵数据
-- iscolmajor == false 以行为主序的矩阵数据
--
function matrix:detach(iscolmajor)
	if iscolmajor == nil or iscolmajor == true then
		local tempm = self:clone()
		return tempm.m_data
	else
		local tempm = self:clone()
		local temp = tempm:t()
		return temp.m_data
	end
end


--
-- 反转矩阵
-- dim == 0 反转列
-- dim == 1 反转行
--
function matrix:reverse(dim)
	return self:slice(-1,dim)
end


-- 
-- 获取复数矩阵的实部
--
function matrix:real()
	local m  = matrix:new()
	if not self.b_init or self.m_real == nil then
		return m
	end
	m.n_cols = self.n_cols
	m.n_rows = self.n_rows
	m.b_init = true
	m.m_data = {}
	for i=1,m.n_cols*m.n_rows do
		m.m_data[i] = self.m_real.m_data[i]
	end
	return  m
end


-- 
-- 获取复数矩阵的虚部
--
function matrix:imag()
	local m  = matrix:new()
	if not self.b_init or self.m_imag == nil then
		return m
	end
	m.n_cols = self.n_cols
	m.n_rows = self.n_rows
	m.b_init = true
	m.m_data = {}
	for i=1,m.n_cols*m.n_rows do
		m.m_data[i] = self.m_imag.m_data[i]
	end
	return  m
end


--
-- 计算复数的模
--
function matrix:abs()
	local m  = matrix:new()
	if not self.b_init  then
		return m
	end
	
	if self.m_data ~= nil then
		m.m_data = {}
		for i=1,#self.m_data do
			m.m_data[i] = math.abs(self.m_data[i])
		end
		m.b_init = true
		m.n_cols = self.n_cols
		m.n_rows = self.n_rows
	else
		if self.m_real ~= nil and self.m_imag ~= nil then
			m.m_data = {}
			for i=1,#self.m_real.m_data do
				local val = math.sqrt(self.m_real.m_data[i]^2+self.m_imag.m_data[i]^2)
				m.m_data[i] = val
			end
			m.b_init = true
			m.n_cols = self.n_cols
			m.n_rows = self.n_rows
		end		
	end
	return m
end


--
-- 获取复数矩阵的相位
--
function matrix:arg()
	local m  = matrix:new()
	if not self.b_init  then
		return m
	end
	
	if self.m_data ~= nil then
		m.m_data = {}
		for i=1,#self.m_data do
			m.m_data[i] = 0
		end
		m.b_init = true
		m.n_cols = self.n_cols
		m.n_rows = self.n_rows
	else
		if self.m_real ~= nil and self.m_imag ~= nil then
			m.m_data = {}
			for i=1,#self.m_real.m_data do
				local val = math.atan(self.m_imag.m_data[i],self.m_real.m_data[i])
				m.m_data[i] = val
			end
			m.b_init = true
			m.n_cols = self.n_cols
			m.n_rows = self.n_rows
		end		
	end
	return m
end


--
-- 常规三角函数的计算
--
function matrix:sin()
	return self:transform(math.sin)
end

function matrix:cos()
	return self:transform(math.cos)
end

function matrix:asin()
	return self:transform(math.asin)
end

function matrix:acos()
	return self:transform(math.acos)
end

function matrix:deg()
	return self:transform(math.deg)
end

function matrix:rad()
	return self:transform(math.rad)
end



-- 
--  一些常规算法
--  所有元素加上指定的数 实部 + real 虚部 + imag
--  num 为指定的数
--
function matrix:plus(real,imag)
	local m1 = self:clone()
	if not self.b_init then
		return m1
	end
	local r0,i0 = m1:cell(1,1)
	if i0 ~= nil and imag ~= nil then
		for i = 1,#self.m_real.m_data do
			m1.m_real.m_data[i] = m1.m_real.m_data[i] + real
			m1.m_imag.m_data[i] = m1.m_imag.m_data[i] + imag
		end
		return m1
	else
		for i=1,#self.m_data do
			m1.m_data[i] = m1.m_data[i] + real
		end
		return m1
	end
end


-- 
--  所有元素减去指定的数 实部 + real 虚部 + imag
--  num 为指定的数
--
function matrix:minus(real,imag)
	local m1 = self:clone()
	if not self.b_init then
		return m1
	end
	local r0,i0 = m1:cell(1,1)
	if i0 ~= nil and imag ~= nil then
		for i = 1,#self.m_real.m_data do
			m1.m_real.m_data[i] = m1.m_real.m_data[i] - real
			m1.m_imag.m_data[i] = m1.m_imag.m_data[i] - imag
		end
		return m1
	else
		for i=1,#self.m_data do
			m1.m_data[i] = m1.m_data[i] - real
		end
	end
end



-- 
--  所有元素除以指定的数 
--  如果当前矩阵为实数矩阵那么实部和虚部分别都除以该数
--  num 为指定的数
--
function matrix:div(num)
	local m1 = self:clone()
	if not self.b_init then
		return m1
	end
	
	
	local fun = function(x)
		return x / num
	end
	
	if self.m_data ~= nil then
		return self:transform(fun)
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		for i = 1,#self.m_real.m_data do
			m1.m_real.m_data[i] = m1.m_real.m_data[i] / num
			m1.m_imag.m_data[i] = m1.m_imag.m_data[i] / num
		end
	end
	return m1
end


-- 
--  所有元素乘以指定的数 
--  如果当前矩阵为实数矩阵那么实部和虚部分别都乘以该数
--  num 为指定的数
--
function matrix:mult(num)
	local m1 = self:clone()
	if not self.b_init then
		return m1
	end
	
	local fun = function(x)
		return x * num
	end
	
	if self.m_data ~= nil then
		return self:transform(fun)
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		for i = 1,#self.m_real.m_data do
			m1.m_real.m_data[i] = m1.m_real.m_data[i] * num
			m1.m_imag.m_data[i] = m1.m_imag.m_data[i] * num
		end
	end
	return m1
end


-- 
--  所有元素将以num为底数进行取log计算
--  该计算不支持复数
--  num 为指定的数
--
function matrix:log(num)
	local fun = function(x)
		if num == 0 then
			return math.log(x)
		else
			return math.log(x,num)
		end
	end
	return self:transform(fun)
end


-- 
--  所有元素将以num为底数进行取log计算
--  取log后的结果再乘以指定的n
--  该计算不支持复数
--  num 为指定的log底数
--  n 取log后需要放大的倍数
--
function matrix:log(num,n)
	local fun = function(x)
		if num == 0 then
			return n*math.log(x)
		else
			return n*math.log(x,num)
		end
	end
	return self:transform(fun)
end


--
-- 以base为底的幂
--  base 为指定的底数
-- 比如 pow(10) --> 10^x
--
function matrix:pow(base)
	local fun = function(x)
		return base^x
	end
	return self:transform(fun)
end

--
--  将数据放大n背后以base为底的幂
--  base 为指定的底数
-- 比如 pow(10,2) --> 10^(x*2)
--
function matrix:pow(base,n)
	local fun = function(x)
		return base^(x*n)
	end
	return self:transform(fun)
end


-- 
-- 获取当前矩阵的行数
--
function matrix:rows()
	if not self.b_init then
		return 0
	end
	return self.n_rows
end



-- 
-- 获取当前矩阵的列数
--
function matrix:cols()
	if not self.b_init then
		return 0
	end
	return self.n_cols
end


-- 
-- 获取当前矩阵的数据大小
--
function matrix:size()
	if not self.b_init then
		return 0
	end
	return self.n_rows*self.n_cols
end


--
-- 矩阵是否为空
--
function matrix:empty()
	if not self.b_init or self.n_rows == nil or self.n_cols == nil then
		return true
	end
	if self.n_rows == 0 or self.n_cols == 0 then
		return true
	end
	
	return false
end	
	
--
-- 获取指定值
-- 获取指定行列号的值
-- rowindex 为指定的行号
-- colindex 为指定的列号
-- 行列号按照lua的原则,从1开始
--
function matrix:get(rowindex,colindex)
	if not self.b_init then
		print("not self.b_init")
		return
	end
	
	if rowindex < 1 or rowindex > self.n_rows then
		print("owindex < 1 or rowindex > self.n_rows")
		return
	end
	
	if colindex < 1 or colindex > self.n_cols then
		print("colindex < 1 or colindex > self.n_cols")
		return
	end
	
	if self.m_data ~= nil then
		return self.m_data[self.n_rows*(colindex-1)+rowindex]
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		return self.m_real.m_data[self.n_rows*(colindex-1)+rowindex],self.m_imag.m_data[self.n_rows*(colindex-1)+rowindex]
	end
	return 0
end


--
-- 设置指定单元的数据
-- 设置指定行列号的值
-- rowindex 为指定的行号
-- colindex 为指定的列号
-- 行列号按照lua的原则,从1开始
--
function matrix:set(rowindex,colindex,val)
	if not self.b_init or self.m_data == nil then
		print("not self.b_init or self.m_data == nil")
		return false
	end
	
	if rowindex < 1 or rowindex > self.n_rows then
		print("rowindex < 1 or rowindex > self.n_rows")
		return false
	end
	
	if colindex < 1 or colindex > self.n_cols then
		print("colindex < 1 or colindex > self.n_cols")
		return false
	end
	
	self.m_data[self.n_rows*(colindex-1)+rowindex] = val
	return true
end

--
-- 设置指定单元的数据
-- 设置指定行列号的值
-- rowindex 为指定的行号
-- colindex 为指定的列号
-- 行列号按照lua的原则,从1开始
--
function matrix:set_cx(rowindex,colindex,real,imag)
	if not self.b_init or self.m_real == nil or self.m_imag == nil then
		print("not self.b_init or self.m_real == nil or self.m_imag == nil")
		return false
	end
	
	if rowindex < 1 or rowindex > self.n_rows then
		print("rowindex < 1 or rowindex > self.n_rows")
		return false
	end
	
	if colindex < 1 or colindex > self.n_cols then
		print("colindex < 1 or colindex > self.n_cols")
		return false
	end
	
	self.m_real.m_data[self.n_rows*(colindex-1)+rowindex] = real
	self.m_imag.m_data[self.n_rows*(colindex-1)+rowindex] = imag
	return true
end


--
-- 单元格数据
-- rowindex 为指定的行号
-- colindex 为指定的列号
-- 行列号按照lua的原则,从1开始
-- real 不为nil 但 imag 为空那么更新的矩阵为实数矩阵
-- real imag 都不为空 那么更新的矩阵为复数矩阵
-- real imag 如果都为空那么为获取数据
--
function matrix:cell(rowindex,colindex,real,imag)
	if real == nil and imag == nil then
		return self:get(rowindex,colindex)
	end
	
	if real ~= nil and imag == nil then
		return self:set(rowindex,colindex,real)
	elseif real ~= nil and imag ~= nil then
		return self:set_cx(rowindex,colindex,real,imag)
	end
end

-- 
-- 求矩阵的转置
--
function matrix:t()
	local __tempm = matrix:new()
	if not self.b_init then
		print("not self.b_init")
		return __tempm
	end
	__tempm.n_rows = self.n_cols
	__tempm.n_cols = self.n_rows
	__tempm.b_init = true
	if self.m_data ~= nil then
		__tempm.m_data = {}
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		__tempm.m_real = matrix:new()
		__tempm.m_imag = matrix:new()
		
		__tempm.m_real.n_cols = __tempm.n_cols
		__tempm.m_real.n_rows = __tempm.n_rows
		__tempm.m_real.b_init = true
		__tempm.m_real.m_data = {}
		
		__tempm.m_imag.n_cols = __tempm.n_cols
		__tempm.m_imag.n_rows = __tempm.n_rows
		__tempm.m_imag.b_init = true
		__tempm.m_imag.m_data = {}
	else
		return __tempm
	end
	
	for i=1,self.n_rows do
		for j=1,self.n_cols do
			__tempm:cell(j,i,self:get(i,j))
		end
	end
	return __tempm
end


-- 
-- 求矩阵的逆
--
function matrix:inv()
	local __tempm = matrix:new()
	if not self.b_init then
		print("not self.b_init or self.m_data == nil")
		return __tempm
	end
	
	__tempm.n_rows = self.n_rows
	__tempm.n_cols = self.n_cols
	
	if self.m_data ~= nil then
	__tempm.b_init = true
		__tempm.m_data = LuaMatrix.inv(self.n_rows,self.n_cols,self.m_data)
		return __tempm
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		local data1,data2 = LuaMatrix.inv_cx(self.n_rows,self.n_cols,self.m_real.m_data,self.m_imag.m_data)
		if data1 == nil or data2 == nil then
			return __tempm
		end
		__tempm.b_init = true
		__tempm.m_real = ones(row,col)
		__tempm.m_imag = ones(row,col)
		__tempm.m_real.m_data = data1
		__tempm.m_imag.m_data = data2
		return __tempm
	end	
end

-- 
-- 求矩阵的行列式
--
function matrix:det()
	if not self.b_init then
		print("not self.b_init ")
		return 0
	end
	if self.m_data ~= nil then
		return LuaMatrix.det(self.n_rows,self.n_cols,self.m_data)
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		return LuaMatrix.det_cx(self.n_rows,self.n_cols,self.m_real.m_data,self.m_imag.m_data)
	end
end

--
-- 求矩阵的最大值
--
function matrix:max()
	if not self.b_init or self.m_data == nil then
		print("not self.b_init or self.m_data == nil")
		return 0
	end
	return LuaMatrix.max(self.n_rows,self.n_cols,self.m_data)
end

-- 
-- 求矩阵的最大值
--
function matrix:min()
	if not self.b_init or self.m_data == nil then
		print("not self.b_init or self.m_data == nil")
		return 0
	end
	return LuaMatrix.min(self.n_rows,self.n_cols,self.m_data)
end


--
-- 获取指定行
--
function matrix:row(index)
	local m = matrix:new()
	if not self.b_init or self.m_data == nil then
		print("not self.b_init or self.m_data == nil")
		return m
	end
	
	if index < 1 or index > self.n_rows then
		return m
	end
	
	m.n_cols = self.n_cols
	m.n_rows = 1
	m.b_init = true;
	m.m_data = {}
	for i = 1,self.n_cols do
		m.m_data[i] = self.m_data[(i-1)*self.n_rows+index]
	end
	return m
end


--
-- 更新指定行
--
function matrix:update_row(index,newrow)
	if not self.b_init or self.m_data == nil or not newrow.b_init then
		print("not self.b_init or self.m_data == nil")
		return false
	end
	
	if index < 1 or index > self.n_rows then
		return false
	end
	
	if newrow.n_cols ~= self.n_cols then
		return false
	end
	
	if newrow.n_rows ~= 1 then
		return false
	end
	
	for i = 1,self.n_cols do
		self.m_data[(i-1)*self.n_rows+index] = newrow.m_data[i]
	end
	return true
end


--
-- 获取指定列
-- 
function matrix:col(index)
	local m = matrix:new()
	if not self.b_init or self.m_data == nil then
		print("not self.b_init or self.m_data == nil")
		return m
	end
	
	if index < 1 or index > self.n_cols then
		return m
	end
	
	m.n_cols = 1
	m.n_rows = self.n_rows
	m.b_init = true;
	m.m_data = {}
	for i = 1,self.n_rows do
		m.m_data[i] = self.m_data[(index - 1)*self.n_rows + i]
	end
	return m
end

--
-- 更新指定列
--
function matrix:update_col(index,newcol)
	if not self.b_init or self.m_data == nil or not newcol.b_init then
		print("not self.b_init or self.m_data == nil")
		return false
	end
	
	if index < 1 or index > self.n_cols then
		return false
	end
	
	if newcol.n_rows ~= self.n_rows or newcol.n_cols ~= 1 then
		return false
	end
	
	for i = 1,self.n_rows do
		self.m_data[(index - 1)*self.n_rows + i] = newcol.m_data[i]
	end
	
	return true
end


--
-- 添加行
--
function matrix:insert_rows(pos,newrow)
	if not self.b_init or self.m_data == nil then
		return false
	end
	if newrow.n_cols ~= self.n_cols or newrow.n_rows == 0 or newrow.m_data == nil then
		return false
	end
	
	local row,col,data = LuaMatrix.insert_rows(self.n_rows,self.n_cols,self.m_data,newrow.n_rows,newrow.n_cols,newrow.m_data,pos)
	
	if row == nil or col == nil or data == nil then
		return false
	end
	
	self.n_cols = col
	self.n_rows = row
	self.m_data = data
	return true
end

--
-- 添加列
--
function matrix:insert_cols(pos,newcol)
	if not self.b_init or self.m_data == nil then
		return false
	end
	if newcol.n_rows ~= self.n_rows or newcol.n_cols == 0 or newcol.m_data == nil  then
		return false
	end
	local row,col,data = LuaMatrix.insert_cols(self.n_rows,self.n_cols,self.m_data,newcol.n_rows,newcol.n_cols,newcol.m_data,pos)
	
	if row == nil or col == nil or data == nil then
		return false
	end
	
	self.n_cols = col
	self.n_rows = row
	self.m_data = data
	return true
end


--
-- 连接一个矩阵 
-- 水平方向上连接
--
function matrix:join_horiz(m1)
	-- 如果当前矩阵没有初始化那么 == 当前矩阵
	if not self.b_init then
		if m1.m_data ~= nil then
			self.m_real = nil
			self.m_imag = nil
			self.n_rows = m1.n_rows
			self.n_cols = m1.n_cols
			self.b_init = true
			self.m_data = {}
			for i=1,#m1.m_data do
				self.m_data[i] = m1.m_data[i]
			end
			return true
		elseif m1.m_real ~= nil and m1.m_imag ~= nil then
			self.m_data = nil
			self.b_init = true
			self.n_rows = m1.n_rows
			self.n_cols = m1.n_cols
			
			self.m_real = matrix:new()
			self.m_real.n_cols = m1.n_cols
			self.m_real.n_rows = m1.n_rows
			self.m_real.b_init = true
			self.m_real.m_data = {}
			
			self.m_imag = matrix:new()
			self.m_imag.n_cols = m1.n_cols
			self.m_imag.n_rows = m1.n_rows
			self.m_imag.b_init = true
			self.m_imag.m_data = {}
			
			for i=1,#m1.m_real.m_data do
				self.m_real.m_data[i] = m1.m_real.m_data[i]
				self.m_imag.m_data[i] = m1.m_imag.m_data[i]
			end
			
			return true
		end
	end
	
	-- 如果当前矩阵已包含数据
	-- 那么判断类型是否相等
	-- 如果类型相等的话可以进行追加
	if self.m_data ~= nil and m1.m_data ~= nil then
		if self.n_rows ~= m1.n_rows then
			return false
		end
		local index = #self.m_data
		for i=1,#m1.m_data do
			self.m_data[index+i] = m1.m_data[i]
		end
		self.n_cols = self.n_cols + m1.n_cols
		return true
	end
	
	
	-- 如果当前是复数矩阵
	-- 那么分别连接实部虚部
	if self.m_real ~= nil and m1.m_real ~= nil and self.m_imag ~= nil and m1.m_imag ~= nil then
		if self.n_rows ~= m1.n_rows then
			return false
		end
		local index = #self.m_real.m_data
		for i=1,#m1.m_real.m_data do
			self.m_real.m_data[index+i] = m1.m_real.m_data[i]
			self.m_imag.m_data[index+i] = m1.m_imag.m_data[i]
		end
		self.n_cols = self.n_cols + m1.n_cols
		self.m_real.n_cols = self.n_cols 
		return true
	end
	
	return false
end


--
-- 连接一个矩阵 
-- 垂直方向上连接
--
function matrix:join_vert(m1)
	local tempm = self:t()
	local tempm2 = m1:t()
	local isok = tempm:join_horiz(tempm2)
	if not isok then
		return false
	end
	
	tempm = tempm:t()
	self.b_init = tempm.b_init
	self.n_rows = tempm.n_rows
	self.n_cols = tempm.n_cols
	self.m_data = {}
	if tempm.m_data ~= nil then
		for i=1,#tempm.m_data do
			self.m_data[i] = tempm.m_data[i]
		end
	elseif tempm.m_real ~= nil and tempm.m_imag ~= nil then
		self.m_real = tempm.m_real:clone()
		self.m_imag = tempm.m_imag:clone()
	end
	return true
end


--
-- shift
-- 将矩阵从中间进行shift旋转
--
function matrix:shift()
	if not self.b_init then
		return false
	end
	
	if self.m_data ~= nil and self.m_real == nil and self.m_imag == nil then
		local row,col,data = LuaMatrix.shift2(self.n_rows,self.n_cols,self.m_data)
		if row == nil or col == nil or data == nil then
			return false
		end
		self.n_rows = row
		self.n_cols = col
		self.m_data = data
		return true
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		local row,col,data = LuaMatrix.shift2(self.n_rows,self.n_cols,self.m_real.m_data)
		if row == nil or col == nil or data == nil then
			return false
		end
		self.m_real.n_rows = row
		self.m_real.n_cols = col
		self.m_real.m_data = data
		
		row = nil
		col = nil
		data = nil
		row,col,data = LuaMatrix.shift2(self.n_rows,self.n_cols,self.m_imag.m_data)
		if row == nil or col == nil or data == nil then
			return false
		end
		self.m_imag.n_rows = row
		self.m_imag.n_cols = col
		self.m_imag.m_data = data
		
		self.n_rows = row
		self.n_cols = col
		return true
	end
	return false
end

--
-- 变换
-- fun是一个变换函数 参数为数字类型,返回的值也为数值类型 
--
function matrix:transform(fun)
	if not self.b_init or self.m_data == nil then
		return false
	end
	
	local  m = matrix:new()
	m.b_init = true
	m.n_cols = self.n_cols
	m.n_rows = self.n_rows
	m.m_data = {}
	
	for i=1,#self.m_data do
		m.m_data[i] = fun(self.m_data[i])
	end
	return m
end


--
-- 按行求和或者按列求和
--
function matrix:sum(iscol)
	if not self.b_init or self.m_data == nil then
		return false
	end
	
	if iscol == nil then
		return LuaMatrix.sum(self.n_rows,self.n_cols,self.m_data)
	end
	
	local m1 = matrix:new()
	if iscol == true then
		m1.n_cols = self.n_cols
		m1.n_rows = 1
		m1.m_data = {}
		m1.b_init = true
		for i=1,self.n_cols  do
			local  val = 0
			for j=1,self.n_rows do
				val = val + self:get(j,i)
			end
			m1.m_data[i] = val
		end
	elseif iscol == false then
		m1.n_cols = 1
		m1.n_rows = self.n_rows
		m1.m_data = {}
		m1.b_init = true
		for i=1,self.n_rows  do
			local  val = 0
			for j=1,self.n_cols do
				val = val + self:get(i,j)
			end
			m1.m_data[i] = val
		end
	end
	return m1
end


--
-- 求平均值
--
function matrix:aval(index,iscol)
	if not self.b_init or self.m_data == nil then
		return 0
	end
	if self:size() == 0 then
		return 0
	end
	
	if index == nil and iscol == nil then
		local __sum = 0
		for i=1,#self.m_data do
			__sum = __sum + self.m_data[i]
		end
		return __sum / self:size()
	elseif index ~= nil and iscol ~= nil and type(iscol) == "boolean" then
		if iscol == true then
			if index < 1 or index > self.n_rows then
				return 0
			end
			local __sum = 0
			for i=1,self.n_rows do
				__sum = __sum + self:cell(i,index)
			end
			return __sum / self.n_rows
		else
			if index < 1 or index > self.n_cols then
				return 0
			end
			local __sum = 0
			for i=1,self.n_cols do
				__sum = __sum + self:cell(index.i)
			end
			return __sum / self.n_cols
		end
	end
	return 0
end


--
-- 映射一个区域
-- srow 起始行
-- erow 终止行
-- scol 起始列
-- ecol	终止列
-- 按照lua的规则 索引都是从1开始
--
function matrix:map(srow,erow,scol,ecol)
	if not self.b_init or self.m_data == nil then
		return false
	end
	local  m = matrix:new()
	local row,col,data = LuaMatrix.map(self.n_rows,self.n_cols,self.m_data,srow,erow,scol,ecol)
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- 映射一个区域
-- startrow 起始行
-- rownum   行数
-- startcol 起始列
-- colnum	列数
-- 按照lua的规则 索引都是从1开始
-- 该接口不但可以对实数矩阵进行裁剪,同时支持复数矩阵的裁剪
--
function matrix:submat(startrow,rownum,startcol,colnum)
	local  m = matrix:new()
	if not self.b_init then
		return m
	end
	
	if startrow < 1 or startrow + rownum > self.n_rows then
		return m
	end
	
	if startcol < 1 or startcol + colnum > self.n_cols then
		return m
	end

	if self:size() == 0 then
		return m
	end
	m.n_cols = colnum
	m.n_rows = rownum
	m.b_init = true
	if self.m_data ~= nil and self.m_real == nil and self.m_imag == nil then
		m.m_data = {}
		m.m_real = nil
		m.m_imag = nil
		local index = 1
		for i=1,self.n_cols do
			if i >= startcol  and i < startcol + colnum then
				for j=1,self.n_cols do
					if j >= startrow and j < startrow + rownum then
						m.m_data[index] = self:get(j,i)
						index = index + 1
					end
				end
			end
		end
	elseif self.m_real ~= nil and self.m_imag ~= nil then
		m.m_data = nil
		m.m_real = matrix:new()
		m.m_imag = matrix:new()
		
		m.m_real.b_init = true
		m.m_real.n_rows = m.n_rows
		m.m_real.n_cols = m.n_cols
		m.m_real.m_data = {}
		
		m.m_imag.b_init = true
		m.m_imag.n_rows = m.n_rows
		m.m_imag.n_cols = m.n_cols
		m.m_imag.m_data = {}
		
		local index = 1
		for i=1,self.n_cols do
			if i >= startcol  and i < startcol + colnum then
				for j=1,self.n_cols do
					if j >= startrow and j < startrow + rownum then
						m.m_real.m_data[index] = self.m_real:get(j,i)
						m.m_imag.m_data[index] = self.m_imag:get(j,i)
						index = index + 1
					end
				end
			end
		end
	end
	return m
end



--
-- 检查是否有inf
-- 如果包含 -inf 或者 inf 的话返回true
-- 
function matrix:has_inf()
	if not self.b_init or self.m_data == nil then
		return false
	end
	return LuaMatrix.has_inf(self.n_rows,self.n_cols,self.m_data)
end


--
-- 检查是否有nan
-- 如果包含有 -nan 或者 nan 都将返回 true
--
function matrix:has_nan()
	if not self.b_init or self.m_data == nil then
		return false
	end
	return LuaMatrix.has_nan(self.n_rows,self.n_cols,self.m_data)
end


-- 
-- 打印
--
function matrix:print()
	if not self.b_init then
		return "not data neet print"
	end
	
	if self.m_data ~= nil then
		LuaMatrix.print(self.n_rows,self.n_cols,self.m_data)
	else
		if self.m_real ~= nil and self.m_imag ~= nil then
			LuaMatrix.printcx(self.n_rows,self.n_cols,self.m_real.m_data,self.m_imag.m_data)
		end
	end
end


-- 
-- 保存
-- filename 字符串类型 需要保存的文件名
-- isbinary bool 类型 是否按照二进制的方式进行储存
--
function matrix:save(filename,isbinary)
	if not self.b_init or self.m_data == nil then
		return "not data neet print"
	end
	return LuaMatrix.save(self.n_rows,self.n_cols,self.m_data,filename,isbinary)
end


--
-- 加载数据
-- filename 数据所在的文件名
-- 该函数加载的数据是save的数据
-- 不需要指定是否为二进制类型，内部会自行检查
--
function matrix:load(filename)
	local m = matrix:new()
	local isok,row,col,data = LuaMatrix.load(filename,true)
	if not isok then
		return m
	end
	
	m.n_cols = col
	m.n_rows = row
	m.b_init = true
	m.m_data = data
	return m
end


-- 
-- 实现元方法
-- 实现两矩阵相加
-- m2 = m1 + m
-- 其实代码可以更加精简一些，比如
-- local r1,i1 = m1:cell(row,col)
-- local r2,i2 = m2:cell(row,col)
-- if i1 ~= nil  and i2 ~= nil then
--  	m:cell(row,col,r1+r2,i1,+i2)
-- elseif i1 ~= nil then
--		m:cell(row,col,r1+r2,i1)
-- elseif i2 ~= nil then
--		m:cell(row,col,r1+r2,i2)
-- else
-- 		m:cell(row,col,r1+r2)
-- end
-- return m
-- 同理其他的方法可以按照此进行精简
-- 为什么这里并没有这么去精简呢
-- 首先这段代码写得比较早
-- 而且能够正常的工作
-- 再加上考虑没必要把时间花在这上面所以就没有进行精简
-- 有时间了再回来精简
--
function matrix.__add(m1,m2)
	local m = matrix:new()
	if m1.n_cols ~= m2.n_cols or m1.n_rows ~= m2.n_rows then
		return m1
	end
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return m1
	end
	
	if m1.m_data ~= nil and m2.m_data ~= nil then
		m:zero(m1.n_rows,m1.n_cols)
		for i=1,m1.n_rows*m1.n_cols do
			m.m_data[i] = m1.m_data[i] + m2.m_data[i]
		end
		return m
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2,i2 = m2:cell(i,j)
					local r3 = r1+r2
					local i3 = i1+i2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2 = m2:cell(i,j)
					local r3 = r1+r2
					local i3 = i1
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m2.m_real ~= nil and m2.m_imag ~= nil and m1.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r2,i2 = m2:cell(i,j)
					local r1 = m1:cell(i,j)
					local r3 = r1+r2
					local i3 = i2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		end
	end
end


--
-- 实现两矩阵相减
-- m2 = m - m1
--
function matrix.__sub(m1,m2)
	local m = matrix:new()
	if m1.n_cols ~= m2.n_cols or m1.n_rows ~= m2.n_rows then
		return m1
	end
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return m1
	end
	
	if m1.m_data ~= nil and m2.m_data ~= nil then
		m:zero(m1.n_rows,m1.n_cols)
		for i=1,m1.n_rows*m1.n_cols do
			m.m_data[i] = m1.m_data[i] - m2.m_data[i]
		end
		return m
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2,i2 = m2:cell(i,j)
					local r3 = r1-r2
					local i3 = i1-i2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2 = m2:cell(i,j)
					local r3 = r1-r2
					local i3 = i1
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m2.m_real ~= nil and m2.m_imag ~= nil and m1.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r2,i2 = m2:cell(i,j)
					local r1 = m1:cell(i,j)
					local r3 = r1-r2
					local i3 = i2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		end
	end
end


--
-- 实现两矩阵元素相乘
-- m2 = m % m1
-- 
function matrix.__mod(m1,m2)
	local m = matrix:new()
	if m1.n_cols ~= m2.n_cols or m1.n_rows ~= m2.n_rows then
		return m1
	end
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return m1
	end
	
	if m1.m_data ~= nil and m2.m_data ~= nil  then
		m:zero(m1.n_rows,m1.n_cols)
		for i=1,m1.n_rows*m1.n_cols do
			m.m_data[i] = m1.m_data[i] * m2.m_data[i]
		end
		return m
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2,i2 = m2:cell(i,j)
					local r3 = (r1*r2 - i1*i2)
					local i3 = (i1*r2 + i2*r1)
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2 = m2:cell(i,j)
					local r3 = r1*r2
					local i3 = i1*r2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m2.m_real ~= nil and m2.m_imag ~= nil and m1.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r2,i2 = m2:cell(i,j)
					local r1 = m1:cell(i,j)
					local r3 = r1*r2
					local i3 = r1*i2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		end
	end
	
end


--
-- 实现两矩阵元素相除
-- m2 = m / m1
--
function matrix.__div(m1,m2)
	local m = matrix:new()
	if m1.n_cols ~= m2.n_cols or m1.n_rows ~= m2.n_rows then
		return m1
	end
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return m1
	end
	
	if m1.m_data ~= nil and m2.m_data ~= nil then
		m:zero(m1.n_rows,m1.n_cols)
		for i=1,m1.n_rows*m1.n_cols do
			m.m_data[i] = m1.m_data[i] / m2.m_data[i]
		end
		return m
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2,i2 = m2:cell(i,j)
					local r3 = (r1*r2 + i1*i2)/(r2^2+i2^2)
					local i3 = (i1*r2 - i2*r1)/(r2^2+i2^2)
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r1,i1 = m1:cell(i,j)
					local r2 = m2:cell(i,j)
					local r3 = r1/r2
					local i3 = i1/r2
					m:cell(i,j,r3,i3)
				end
			end
			return m
		elseif m2.m_real ~= nil and m2.m_imag ~= nil and m1.m_data ~= nil then
			local r = zeros(m1.n_rows,m1.n_cols)
			local c = zeros(m1.n_rows,m1.n_cols)
			m:cx(r,c)
			for i=1,m1.n_rows do
				for j=1,m1.n_cols do
					local r2,i2 = m2:cell(i,j)
					local r1 = m1:cell(i,j)
					local r3 = (r1*r2)/(r2^2+i2^2)
					local i3 = (-i2*r1)/(r2^2+i2^2)
					m:cell(i,j,r3,i3)
				end
			end
			return m
		end
	end
end


--
-- 实现矩阵的叉乘
-- 按照矩阵的乘法规则进行相乘
--
function matrix.__mul(m1,m2)
	local m = matrix:new()
	if m1.n_cols ~= m2.n_rows then
		return
	end
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return
	end
	
	if m2.n_rows == 0 or m1.n_cols == 0 then
		return
	end
	
	if m1.m_data ~= nil and m2.m_data ~= nil then
		local row,col,data = LuaMatrix.mult(m1.n_rows,m1.n_cols,m1.m_data,m2.n_rows,m2.n_cols,m2.m_data)
		if row == 0 or col == 0 then
			return
		end
		m.n_rows = row
		m.n_cols = col
		m.m_data = data
		m.b_init = true
		return m
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local row,col,data1,data2 = LuaMatrix.mult_cx(m1.n_rows,m1.n_cols,m1.m_real.m_data,m1.m_imag.m_data,m2.n_rows,m2.n_cols,m2.m_real.m_data,m2.m_imag.m_data)
			if row == nil or col == nil or data1 == nil or data2 == nil then
				return
			end
			m.n_rows = row
			m.n_cols = col
			m.b_init = true
			m.m_real = zeros(row,col)
			m.m_imag = zeros(row,col)
			m.m_real.m_data = data1
			m.m_imag.m_data = data2
			return m
		elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
			local tempm = zeros(m2.n_rows,m2.n_cols)
			local cm = complex(m2.n_rows,m2.n_cols)
			cm:cx(m2,tempm)
			local row,col,data1,data2 = LuaMatrix.mult_cx(m1.n_rows,m1.n_cols,m1.m_real.m_data,m1.m_imag.m_data,cm.n_rows,cm.n_cols,cm.m_real.m_data,cm.m_imag.m_data)
			if row == nil or col == nil or data1 == nil or data2 == nil then
				return
			end
			m.n_rows = row
			m.n_cols = col
			m.b_init = true
			m.m_real = zeros(row,col)
			m.m_imag = zeros(row,col)
			m.m_real.m_data = data1
			m.m_imag.m_data = data2
			return m
		elseif m1.m_data ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
			local tempm = zeros(m1.n_rows,m1.n_cols)
			local cm = complex(m1.n_rows,m1.n_cols)
			cm:cx(m1,tempm)
			local row,col,data1,data2 = LuaMatrix.mult_cx(cm.n_rows,cm.n_cols,cm.m_real.m_data,cm.m_imag.m_data,m2.n_rows,m2.n_cols,m2.m_real.m_data,m2.m_imag.m_data)
			if row == nil or col == nil or data1 == nil or data2 == nil then
				return
			end
			m.n_rows = row
			m.n_cols = col
			m.b_init = true
			m.m_real = zeros(row,col)
			m.m_imag = zeros(row,col)
			m.m_real.m_data = data1
			m.m_imag.m_data = data2
			return m
		end
	end
end


--
-- 实现print打印
--
function matrix.__tostring(m1)
	if m1.n_cols == 0 or m1.n_rows == 0 then
		return ""
	end
	
	if m1.m_data ~= nil then
		return LuaMatrix.tostring(m1.n_rows,m1.n_cols,m1.m_data)
	else
		if m1.m_real ~= nil and m1.m_imag ~= nil then
			return LuaMatrix.tostring2(m1.n_rows,m1.n_cols,m1.m_real.m_data,m1.m_imag.m_data)
		end
	end
	return ""
end


--
-- pow的元方法
--
function matrix.__pow(m1,n)
	if not m1.b_init then
		return
	end
	
	local r0,i0 = m1:cell(1,1)
	if r0 == nil and i0 == nil then
		return
	end
	
	if i0 ~= nil then
		return
	else
		local m = zeros(m1.n_rows,m1.n_cols)
		for i=1,#m1.m_data do
			m.m_data[i] = m1.m_data[i]^n
		end
		return m
	end
end


--
-- 判断两个矩阵是否相等
--
function matrix.__eq(m1,m2)
	if not m1.b_init or not m2.b_init then
		return false
	end
	
	if m1.m_data ~= nil and m2.m_data == nil then
		return false
	end
	
	if m1.m_real ~= nil and m2.m_real == nil then
		return false
	end
	
	if m1.n_rows ~= m2.n_rows or m1.n_cols ~= m2.n_cols then
		return false
	end
	
	--
	-- 实数矩阵的判断
	--
	if m1.m_data ~= nil then
		for i=1,#m1.m_data do
			if math.abs(m1.m_data[i] - m2.m_data[i]) > 1e-5 then
				return false
			end
		end
		return true
	elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_imag ~= nil then
		--
		-- 复数矩阵通过实数矩阵来判断
		--
		if m1.m_real ~= m2.m_real or m1.m_imag ~= m2.m_imag then
			return false
		end
		return true
	end
	return false	
end


--[[-------------------------------------------------------提供一些线性代数的计算-------------------------------------]]
--
-- linearity 的实现
-- 给定起始，终止，点数  生成一个系列
--
function linearity:range(start,stop,counts)
	local m = matrix:new()
	m.n_cols = 1
	m.n_rows = counts
	m.b_init = true
	m.m_data  = LuaMatrix.range(start,stop,counts)
	return m
end


--
-- 线性插值
--
function linearity:interp1(x,y,disx)
	local m = matrix:new()
	if x.m_data == nil or y.m_data == nil or disx.m_data == nil then
		return m
	end
	
	if x.n_rows ~= y.n_rows or x.n_cols ~= y.n_cols then
		return m
	end
	local data = LuaMatrix.interp1(x.m_data,y.m_data,disx.m_data)
	if data == nil then
		return m
	end
	m.n_cols = 1
	m.n_rows = disx.n_rows
	m.b_init = true
	m.m_data = data
	return m
end


--
-- spline 插值
--
function linearity:spline1(x,y,disx)
	local m = matrix:new()
	if x.m_data == nil or y.m_data == nil or disx.m_data == nil then
		return m
	end
	
	if (x.n_cols == 1 and x.n_rows == 1) or (x.n_cols == 0 and x.n_rows == 0)  then
		return m
	end
	
	if x.n_rows ~= y.n_rows or x.n_cols ~= y.n_cols then
		return m
	end
	
	if x.n_rows ~= 1 and x.n_cols ~= 1 then
		return m
	end
	
	if disx.n_cols  ~= 1 and disx.n_rows ~= 1 then
		return m
	end
	
	if (disx.n_cols == 1 and disx.n_rows == 1) or (disx.n_cols == 0 and disx.n_rows == 0) then
		return m
	end
	
	local row,col,data = LuaMatrix.spline1(#(x.m_data),x.m_data,y.m_data,#(disx.m_data),disx.m_data)
	if data == nil then
		return m
	end
	m.n_cols = col
	m.n_rows = 1
	m.b_init = true
	m.m_data = data
	return m
end

--
-- spline 插值 对矩阵进行插值
--
function linearity:spline2(x,y,z,disx,disy)
	local m = matrix:new()
	if x.m_data == nil or y.m_data == nil or disx.m_data == nil  or disy.m_data == nil or z.m_data == nil then
		return m
	end
	
	if (x.n_cols == 1 and x.n_rows == 1) or (x.n_cols == 0 and x.n_rows == 0)  then
		return m
	end
	
	if (disx.n_cols == 1 and disx.n_rows == 1) or (disx.n_cols == 0 and disx.n_rows == 0) then
		return m
	end
	
	if (disy.n_cols == 1 and disy.n_rows == 1) or (disy.n_cols == 0 and disy.n_rows == 0) then
		return m
	end
	
	if z.n_cols ~= #x.m_data or z.n_rows ~= #y.m_data then
		return m
	end
	
	if #disx.m_data ~= disy.n_cols then
		return m
	end
	
	local row,col,data = LuaMatrix.spline2(z.n_rows,z.n_cols,x.m_data,y.m_data,z.m_data,#(disx.m_data),disy.n_rows,disx.m_data,disy.m_data)
	if data == nil then
		return m
	end
	m.n_cols = col
	m.n_rows = 1
	m.b_init = true
	m.m_data = data
	return m
end

--
-- 卷积计算
-- x 矩阵
-- y 矩阵
-- samesize bool 类型，指定返回矩阵是否和x一样大
-- samesize == false  返回矩阵大小是 x +  y - 1
-- 
function linearity:conv(x,y,samesize)
	local m = matrix:new()
	if x.m_data == nil or y.m_data == nil  then
		return m
	end
	
	local row,col,data = LuaMatrix.conv(x.n_rows,x.n_cols,x.m_data,y.n_rows,y.n_cols,y.m_data,samesize)
	if data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- fft 实数FFT变换
--
function linearity:fft(x,fftn,fftm)
	
	if not x.b_init then
		return m
	end
	
	local r0,i0 = x:cell(1,1)
	
	local temp = x:clone()
	if i0 == nil then
		local __temp = x:clone()
		temp.m_data = {}
		temp.m_real = __temp
		temp.m_imag = zeros(x:rows(),x:cols())
	end
	
	local row,col,data1,data2 = LuaMatrix.fft(x.n_rows,x.n_cols,temp.m_real.m_data,temp.m_imag.m_data,fftn,fftm)
	if row == nil or col == nil or data1 == nil or data2 == nil  then
		return
	end
	
	if row == 0 then
		row = 1
	end
	
	if col == 0 then
		col = 1
	end
	
	local m = mat()
	m.m_real = matrix:new()
	m.m_imag = matrix:new()
	
	m.m_real.b_init = true
	m.m_real.n_cols = col
	m.m_real.n_rows = row
	m.m_real.m_data = {}
	for i=1,#data1 do
		m.m_real.m_data[i] = data1[i]
	end
	
	m.m_imag.b_init = true
	m.m_imag.n_cols = col
	m.m_imag.n_rows = row
	m.m_imag.m_data = {}
	for i=1,#data2 do
		m.m_imag.m_data[i] = data2[i]
	end
	
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	return m
end


--
-- 复数FFT变换
--
function linearity:ifft(x,fftm,fftn)
	local m = matrix:new()
	if not x.b_init or x.m_real == nil  or x.m_imag == nil then
		return m
	end
	
	local row,col,data1,data2 = LuaMatrix.ifft(x.n_rows,x.n_cols,x.m_real.m_data,x.m_imag.m_data,fftn,fftm)
	if row == nil or col == nil or data1 == nil or data2 == nil  then
		return m
	end
	
	if row == 0 then
		row = 1
	end
	
	if col == 0 then
		col = 1
	end
	
	m.m_real = matrix:new()
	m.m_imag = matrix:new()
	
	m.m_real.b_init = true
	m.m_real.n_cols = col
	m.m_real.n_rows = row
	m.m_real.m_data = {}
	for i=1,#data1 do
		m.m_real.m_data[i] = data1[i]
	end
	
	m.m_imag.b_init = true
	m.m_imag.n_cols = col
	m.m_imag.n_rows = row
	m.m_imag.m_data = {}
	for i=1,#data2 do
		m.m_imag.m_data[i] = data2[i]
	end
	
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	return m
end


--
-- 计算多项式系数
-- y = p0xN + p1xN-1 + p2xN-2 + ... + pN-1x1 + pN 
-- 通过 x,y 计算出个系数 p
--
function linearity:polyfit(x,y,n)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	if x.n_cols == 0 and x.n_rows == 0 then
		return m
	end
	
	if x.n_cols ~= 1 and x.n_rows ~= 1  then
		return m
	end
	
	if x.n_cols ~= y.n_cols and x.n_rows ~= y.n_rows then
		return m
	end
	
	local data = LuaMatrix.polyfit(x.m_data,y.m_data,n)
	
	if data == nil then
		return m
	end
	
	m.b_init = true
	m.n_cols = #data
	m.n_rows = 1
	m.m_data = data
	return m
end

--
-- 计算多项式
-- y = p0xN + p1xN-1 + p2xN-2 + ... + pN-1x1 + pN 
-- 通过 x,p 计算出结果 y
--
function linearity:polyval(x,y)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	if x.n_cols == 0 and x.n_rows == 0 then
		return m
	end
	
	if x.n_cols ~= 1 and x.n_rows ~= 1  then
		return m
	end
	
	if y.n_cols == 0 and y.n_rows == 0 then
		return m
	end
	
	if y.n_cols ~= 1 and y.n_rows ~= 1  then
		return m
	end
	
	local data = LuaMatrix.polyval(x.m_data,y.m_data)
	
	if data == nil then
		return m
	end
	
	m.b_init = true
	m.n_cols = #data
	m.n_rows = 1
	m.m_data = data
	return m
end

--
-- 解矩阵方程
-- A*X = B
-- A 可以式一个正方形矩阵也可以不是
-- B 的行必须等于A的行
--
function linearity:solve(x,y)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	if x.n_rows ~= y.n_rows then
		return m
	end
	
	if x.n_cols == 0 or x.n_rows == 0 then
		return m
	end

	if y.n_cols == 0 or y.n_rows == 0 then
		return m
	end
	
	local row,col,data = LuaMatrix.solve(x.n_rows,x.n_cols,y.n_cols,x.m_data,y.m_data)
	
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	m.m_data = data
	return m
end

--
-- join_horiz 水平连接
-- 仅仅支持实数矩阵
--
function linearity:join_horiz(x,y)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	if x.n_rows ~= y.n_rows then
		return m
	end
	
	if x.n_cols == 0 or x.n_rows == 0 then
		return m
	end

	if y.n_cols == 0 or y.n_rows == 0 then
		return m
	end
	
	local row,col,data = LuaMatrix.join_horiz(x.n_rows,x.n_cols,x.m_data,y.n_rows,y.n_cols,y.m_data)
	
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	m.m_data = data
	return m
end


--
-- 垂直连接
-- 仅仅支持实数矩阵
--
function linearity:join_vert(x,y)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	if x.n_cols ~= y.n_cols then
		return m
	end
	
	if x.n_cols == 0 or x.n_rows == 0 then
		return m
	end

	if y.n_cols == 0 or y.n_rows == 0 then
		return m
	end
	
	local row,col,data = LuaMatrix.join_vert(x.n_rows,x.n_cols,x.m_data,y.n_rows,y.n_cols,y.m_data)
	
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	m.m_data = data
	return m
end


--
-- 获取一个矩阵的迹
--
function linearity:trace(x)
	if not x.b_init or x.m_data == nil then
		return 0
	end
	
	local val = LuaMatrix.trace(x.n_rows,x.n_rows,x.m_data)
	return val
end


--
-- 唯一化
--
function linearity:unique(x)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil then
		return m
	end
	
	--
	-- 只对实数矩阵有效
	--
	local row,col,data = LuaMatrix.unique(x.n_rows,x.n_rows,x.m_data)
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
--  shift
--  旋转数据
--  比matrix自带的旋转更加灵活自由一些
--  N 需要旋转的元素个数
--  dim 维度 0表示只针对 列进行数据旋转
--  dim 维度 1表示只针对 行进行数据旋转
--
function linearity:shift(x,N,dim)
	local m = matrix:new()
	if not x.b_init then
		return m
	end
	
	--
	-- 如果当前矩阵是实数矩阵
	--
	if x.m_data ~= nil and x.m_real == nil and x.m_imag == nil then
		local row,col,data = LuaMatrix.shift(x.n_rows,x.n_cols,x.m_data,N,dim)

		if row == nil or col == nil or data == nil then
			return m
		end
	
		m.b_init = true
		m.n_rows = row
		m.n_cols = col
		m.m_data = data
		m.m_real = nil
		m.m_imag = nil
		
		return m
	elseif x.m_real ~= nil and x.m_imag ~= nil then
		--
		-- 当前矩阵为复矩阵
		--
		local row,col,data = LuaMatrix.shift(x.n_rows,x.n_cols,x.m_real.m_data,N,dim)
		if row == nil or col == nil or data == nil then
			return m
		end
	
		m.b_init = true
		m.n_rows = row
		m.n_cols = col
		m.m_data = nil
		
		m.m_real = matrix:new()
		m.m_real.n_rows = row
		m.m_real.n_cols = col
		m.m_real.m_data = data
		m.m_real.b_init = true
		
		row,col,data = LuaMatrix.shift(x.n_rows,x.n_cols,x.m_imag.m_data,N,dim)
		if row == nil or col == nil or data == nil then
			return m
		end
		
		m,m_imag = matrix:new()
		m.m_imag.n_rows = row
		m.m_imag.n_cols = col
		m.m_imag.m_data = data
		m.m_imag.b_init = true
		return m
	end
	
	return m
end


--
-- shuffle
-- 按随机顺序重新排序
--
function linearity:shuffle(x,dim)
	local m = matrix:new()
	if not x.b_init then
		return m
	end
	
	--
	-- 如果当前矩阵是实数矩阵
	--
	if x.m_data ~= nil and x.m_real == nil and x.m_imag == nil then
		local row,col,data = LuaMatrix.shuffle(x.n_rows,x.n_rows,x.m_data,dim)
		if row == nil or col == nil or data == nil then
			return m
		end
	
		m.b_init = true
		m.n_rows = row
		m.n_cols = col
		m.m_data = data
		return m
	
	elseif x.m_real ~= nil and x.m_imag ~= nil then
		--
		-- 当前矩阵为复矩阵
		--
		local row,col,data = LuaMatrix.shuffle(x.n_rows,x.n_cols,x.m_real.m_data,N,dim)
		if row == nil or col == nil or data == nil then
			return m
		end
	
		m.b_init = true
		m.n_rows = row
		m.n_cols = col
		m.m_data = nil
		
		m.m_real = matrix:new()
		m.m_real.n_rows = row
		m.m_real.n_cols = col
		m.m_real.m_data = data
		m.m_real.b_init = true
		
		row,col,data = LuaMatrix.shuffle(x.n_rows,x.n_cols,x.m_imag.m_data,N,dim)
		if row == nil or col == nil or data == nil then
			return m
		end
		
		m,m_imag = matrix:new()
		m.m_imag.n_rows = row
		m.m_imag.n_cols = col
		m.m_imag.m_data = data
		m.m_imag.b_init = true
	end
	
	return m
end


--
-- sort
-- 对矩阵进行排序
-- isascend true --> 从小到大
-- dim 0 列 1 行
--
function linearity:sort(x,isascend,dim)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil then
		return m
	end
	
	--
	-- 只对实数矩阵有效
	--
	local row,col,data = LuaMatrix.sort(x.n_rows,x.n_cols,x.m_data,isascend,dim)
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- 获取一个矩阵的上三角
-- 下三角形使用0填充
--
function linearity:trimatu(x)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil then
		return m
	end
	
	local row,col,data = LuaMatrix.trimatu(x.n_rows,x.n_rows,x.m_data)
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- 获取一个矩阵的下三角
-- 上三角形部分使用0填充
--
function linearity:trimatl(x)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil then
		return m
	end
	
	local row,col,data = LuaMatrix.trimatl(x.n_rows,x.n_rows,x.m_data)
	if row == nil or col == nil or data == nil then
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- 求有限积分
-- x 是一列或者一行数据
-- y 可以是一行或者一列或者一个矩阵
-- dim 0 列 1 行
--
function linearity:trapz(x,y,dim)
	local m = matrix:new()
	if not x.b_init or x.m_data == nil or not y.b_init or y.m_data == nil then
		return m
	end
	
	local row,col,data = LuaMatrix.trapz(y.n_rows,y.n_cols,y.m_data,x.m_data,dim)
	if row == nil or col == nil or data == nil then
		print(row,col,data)
		return m
	end
	
	m.b_init = true
	m.n_rows = row
	m.n_cols = col
	m.m_data = data
	return m
end


--
-- cholesky 分解
-- 把一个对称正定的矩阵表示成一个下三角矩阵L和其转置的乘积的分解
-- 要求矩阵的所有特征值必须大于0
-- 所以分解的下三角的对角元也是大于0的
-- cholesky 分解法又称为平方根法，是当A为实对称正定矩阵时，LU三角分解的变形
-- isupper == 分解的结果为上叫三角 [和上面的描述是有其别的,因为这是程序嘛]
--
function chol(m,isupper)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end
	if isupper == nil or type(isupper) ~= "boolean" then
		isupper = false
	end
	if r0 ~= nil and i0 == nil  then
		local row,col,data = LuaMatrix.chol(m:rows(),m:cols(),m.m_data,isupper)
		if row == nil or col == nil or data == nil then
			return
		end
		local result = mat()
		result:attach(data1,row,col)
		return result
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2= LuaMatrix.chol_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data,isupper)
		if row == nil or col == nil or data1 == nil or data2 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		return result
	end
end


--
-- 计算矩阵的特征值以及特征向量
-- M*X = v*X
-- M 为 n 阶方阵
-- X 为特征向量
-- v 为特征值
-- f(v) = v*E - M
-- f(v) = 0 的解便是特征值
-- 特征值对应的向量叫特征向量
-- isstd == true 使用标准方法
-- isstd == false 使用divide-and-conquer
-- 默认是false
-- 该算法使用对称正定矩阵
-- 如果是非正定矩阵的话使用 eig_gen 算法
--
--
function linearity:eig_sym(m,isstd)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end
	if isstd == nil or  type(isstd) ~= "boolean" then
		isstd = false
	end
	
	if r0 ~= nil and i0 == nil  then
		local row,col,data1,data2 = LuaMatrix.eig_sym(m:rows(),m:cols(),m.m_data,isstd)
		if row == nil or col == nil or data1 == nil or data2 == nil then
			return
		end
		local result = mat()
		result:attach(data1,row,col)
		local result2 = mat()
		result2:attach(data2,1,#data2)
		return result,result2
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,data3= LuaMatrix.eig_sym_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data,isstd)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		local result2 = mat()
		result2:attach(data2,1,#data2)
		return result,result2
	end
end

--
-- 特征值分解
--
function linearity:eig_gen(m)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	if r0 ~= nil and i0 == nil  then
		local row,col,data1,data2,data3,data4 = LuaMatrix.eig_gen(m:rows(),m:cols(),m.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,1,#data3)
		c:attach(data4,1,#data4)
		local result2 = mat()
		result2:cx(r,c)
		return result,result2
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,data3,data4 = LuaMatrix.eig_gen_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,1,#data3)
		c:attach(data4,1,#data4)
		local result2 = mat()
		result2:cx(r,c)
		return result,result2
	end
end


--
-- LU 分解
-- 将矩阵分解为上下两三角阵
-- L*U = M
--
function linearity:lu(m)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	
	if r0 ~= nil and i0 == nil  then
		local row,col,data1,row2,col2,data2 = LuaMatrix.lu(m:rows(),m:cols(),m.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or row2 == nil or col2 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row2,col2)
		return r,c
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,row2,col2,data3,data4 = LuaMatrix.lu_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil or row2 == nil or col2 == nil then
			return
		end
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,row2,col2)
		c:attach(data4,row2,col2)
		local result2 = mat()
		result2:cx(r,c)
		return result,result2
	end
end

--
-- LUP 分解
-- 将矩阵分解为上下两三角阵 以及一个P矩阵
-- P:t()*L*U = M
--
function linearity:lup(m)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	
	if r0 ~= nil and i0 == nil  then
		local row,col,data1,row2,col2,data2,row3,col3,data3 = LuaMatrix.lup(m:rows(),m:cols(),m.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or row2 == nil or col2 == nil then
			return
		end
		
		if row3 == nil or col3 == nil or data3 == nil then
			return
		end
		
		local r = mat()
		local c = mat()
		local p = mat()
		r:attach(data1,row,col)
		c:attach(data2,row2,col2)
		p:attach(data3,row3,col3)
		return r,c,p
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,row2,col2,data3,data4,row3,col3,data5,data6 = LuaMatrix.lup_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil or row2 == nil or col2 == nil then
			return
		end
		
		if row3 == nil or col3 == nil or data5 == nil or data6 == nil then
			return
		end
		
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,row2,col2)
		c:attach(data4,row2,col2)
		local result2 = mat()
		result2:cx(r,c)
		
		r:attach(data5,row3,col3)
		c:attach(data6,row3,col3)
		local result3 = mat()
		result3:cx(r,c)
		return result,result2,result3
	end
end


--
-- QR 分解
-- 将矩阵分解为一个正交矩阵和一个右三角矩阵
-- Q*R = M
--
function linearity:qr(m)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	
	if r0 ~= nil and i0 == nil  then
		local row,col,data1,row2,col2,data2 = LuaMatrix.qr(m:rows(),m:cols(),m.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or row2 == nil or col2 == nil then
			return
		end
		
		local q = mat()
		local r = mat()
		q:attach(data1,row,col)
		r:attach(data2,row2,col2)
		return q,r
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,row2,col2,data3,data4 = LuaMatrix.qr_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil or row2 == nil or col2 == nil then
			return
		end
		
		
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,row2,col2)
		c:attach(data4,row2,col2)
		local result2 = mat()
		result2:cx(r,c)
		
		return result,result2
	end
end

--
-- QR Economical 分解
-- 将矩阵分解为一个正交矩阵和一个右三角矩阵
-- Q*R = M
--
function linearity:qr_econ(m)
	local r0,i0 = m:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	
	if r0 ~= nil and i0 == nil  then
		local row,col,data1,row2,col2,data2 = LuaMatrix.qr_econ(m:rows(),m:cols(),m.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or row2 == nil or col2 == nil then
			return
		end
		
		local q = mat()
		local r = mat()
		q:attach(data1,row,col)
		r:attach(data2,row2,col2)
		return q,r
	elseif r0 ~= nil and i0 ~= nil then
		local row,col,data1,data2,row2,col2,data3,data4 = LuaMatrix.qr_econ_cx(m:rows(),m:cols(),m.m_real.m_data,m.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil or row2 == nil or col2 == nil then
			return
		end
		
		
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,row2,col2)
		c:attach(data4,row2,col2)
		local result2 = mat()
		result2:cx(r,c)
		
		return result,result2
	end
end

--
-- QZ 分解
-- A,B,Q,Z
-- m1 = Q:t()*A*Z:t()
-- m2 = Q:t()*B*Z:t()
--
function linearity:qz(m1,m2)
	local r0,i0 = m1:cell(1,1)
	local r1,i1 = m2:cell(1,1)
	if r0 == nil and i0 == nil then
		print("argaments is invalide")
		return
	end

	if r1 == nil and i1 == nil then
		print("argaments is invalide")
		return
	end

	
	if r0 ~= nil and i0 == nil  and r1 ~= nil and i1 == nil then
		local row,col,data1,row2,col2,data2,row3,col3,data3,row4,col4,data4 = LuaMatrix.qz(m1:rows(),m1:cols(),m1.m_data,m2:rows(),m2:cols(),m2.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or row2 == nil or col2 == nil then
			return
		end
		
		if row3 == nil or col3 == nil or row4 == nil or col4 == nil  or data3 == nil or data4 == nil then
			return
		end
		
		local a = mat()
		local b = mat()
		local q = mat()
		local z = mat()
		a:attach(data1,row,col)
		b:attach(data2,row2,col2)
		q:attach(data3,row3,col3)
		z:attach(data4,row4,col4)
		return a,b,q,z
	elseif r0 ~= nil and i0 ~= nil and r1 ~= nil and i1 ~= nil then
		local row,col,data1,data2,row2,col2,data3,data4,row3,col3,data5,data6,row4,col4,data7,data8 = LuaMatrix.qz_cx(m1:rows(),m1:cols(),m1.m_real.m_data,m1.m_imag.m_data,m2:rows(),m2:cols(),m2.m_real.m_data,m2.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil or data3 == nil or data4 == nil or row2 == nil or col2 == nil then
			return
		end
		
		if row3 == nil or col3 == nil or data5 == nil or data6 == nil or data7 == nil or data8 == nil or row4 == nil or col4 == nil then
			return
		end
		
		local r = mat()
		local c = mat()
		r:attach(data1,row,col)
		c:attach(data2,row,col)
		local result = complex(r,c)
		
		r:attach(data3,row2,col2)
		c:attach(data4,row2,col2)
		local result2 = mat()
		result2:cx(r,c)
		
		r:attach(data5,row3,col3)
		c:attach(data6,row3,col3)
		local result3 = mat()
		result4:cx(r,c)
		
		r:attach(data7,row4,col4)
		c:attach(data8,row4,col4)
		local result4 = mat()
		result4:cx(r,c)
		
		return result,result2,result3,result4
	end
end

--
-- 解方程
-- A*X + X*B + C  = 0
--
function linearity:syl(A,B,C)
	r0,i0 = A:cell(1,1)
	r1,i1 = B:cell(1,1)
	r2,i2 = C:cell(1,1)
	
	if r0 == nil and i0 == nil then
		return
	end
	
	if r1 == nil and i1 == nil then
		return
	end
	
	if r2 == nil and i2 == nil then
		return 
	end
	
	if i0 == nil and i1 == nil and i2 == nil then
		local c1 = mat()
		local zm = zeros(A:rows(),A:cols())
		c1:cx(A,zm)
		local c2 = mat()
		zm = zeros(B:rows(),B:cols())
		c2:cx(B,zm)
		local c3 = mat()
		zm = zeros(C:rows(),C:cols())
		c3:cx(C,zm)
		local row,col,data1,data2 = LuaMatrix.syl(A:rows(),A:cols(),A.m_data,c1.m_imag.m_data,B:rows(),B:cols(),B.m_data,c2.m_imag.m_data,C:rows(),C:cols(),C.m_data,c3.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil  then
			return
		end	
		local a = mat()
		local b = mat()
		a:attach(data1,row,col)
		return a
	elseif i0 ~= nil and i1 ~= nil and i2 ~= nil then
		local row,col,data1,data2 = LuaMatrix.syl(A:rows(),A:cols(),A.m_real.m_data,A.m_imag.m_data,B:rows(),B:cols(),B.m_real.m_data,B.m_imag.m_data,C:rows(),C:cols(),C.m_real.m_data,C.m_imag.m_data)
		if row == nil or col == nil or data1 == nil or data2 == nil then
			return
		end	
		local a = mat()
		local b = mat()
		a:attach(data1,row,col)
		b:attach(data2,row,col)
		return complex(a,b)
	end
end

--[[ --------------------------------------一些全局的工厂函数--------------------------------------------]]
--
-- 工厂函数
-- 新建一个matrix
--
function mat()
	return matrix:new()
end


--
-- 工厂函数
-- 创建一个均匀分布的随机矩阵
--
function randu(row,col)
	local m = matrix:new()
	m:randu(row,col)
	return m
end


--
-- 工厂函数
-- 创建一个正太分布的随机矩阵
--
function randn(row,col)
	local m = matrix:new()
	m:randn(row,col)
	return m
end


--
-- 工厂函数
-- 创建一个1矩阵
--
function ones(row,col)
	local m = matrix:new()
	m:ones(row,col)
	return m
end

--
-- 工厂函数
-- 创建一个0矩阵
--
function zeros(row,col)
	local m = matrix:new()
	m:zero(row,col)
	return m
end


--
-- 工厂函数
-- 创建一个单位矩阵
--
function identify(row,col)
	local m = matrix:new()
	m:identify(row,col)
	return m
end

--
-- 使用一段table来填充
--
function array(t,row,col)
	if type(t) ~= "table" then
		return
	end
	
	local num = #t
	local m = matrix:new()
	m.b_init = true
	m.m_data = {}
	m.n_rows = num
	m.n_cols = 1
	for i = 1,num do
		m.m_data[i] = t[i]
	end
	
	if row ~= nil and col ~= nil then
		m.n_rows = row
		m.n_cols = col
	end
	return m
end


--
-- 使用给定的向量作为对角线
--
function diagmat(vec)
	if type(t) ~= "table" then
		return
	end
	local l = #vec
	local m = zeros(l,l)
	local index = 1
	for i=1,l do
		for j=1,l do
			if i==j then
				m:cell(i,j,vec[index])
				index = index + 1
			end
		end
	end
	return m
end


--
-- 工厂函数
-- 创建一个复矩阵
-- real 实部 
-- imag 虚部
-- 如果real表示实部的时候 那么 real 是一个矩阵
-- 如果real表示行数的话 real是一个数字
-- imag同理
-- 类型为矩阵是将会用该两个矩阵去创建复数矩阵
-- 类型是数字时创建一个0的复数矩阵
-- 如果想要创建一个随机数的复数矩阵
-- c = complex(4,4)
-- c:randu()
-- 此时 c 是一个4行4列的随机数复矩阵
--
function complex(real,imag)
	local m = matrix:new()
	if type(real) == "number" and type(imag) == "number" then
		m:cx(zeros(real,imag),zeros(real,imag))
	else
		m:cx(real,imag)
	end
	return m
end


-- 
-- 工厂函数
-- 创建N阶傅里叶矩阵
-- sig 符号
-- sig > 0 正号
-- sig < 0 符号
--
function fftm(N,sig)
	if sig == nil or type(sig) ~= "number" or sig < 0then
		sig = -1
	end
	
	if sig > 0 then
		sig = 1
	end
	
	local r = ones(N,N)
	local c = zeros(N,N)
	local m = matrix:new()
	m:cx(r,c)
	local theta = 2*math.pi/N
	for i=1,N do
		for j=1,N do
			local angle = (i - 1)*(j - 1)*theta
			m:cell(i,j,math.cos(angle),sig*math.sin(angle))
		end
	end
	return m
end

-- 
-- 工厂函数
-- 创建N阶傅里叶逆矩阵
--
function ifftm(N)
	return fftm(N,1):div(N)
end

--
-- 工厂函数
-- 放大多少倍
--
function ncol(m,n)
	if not m.b_init  then
		return m
	end
	
	local m1 = m:clone()
	local index = m:size()
	if m.m_data ~= nil then
		for i=1,n do
			for j=1,#m1.m_data do
				m.m_data[index + (i-1)*index + j] = m1.m_data[j]
			end
		end
	elseif m.m_real ~= nil and m.m_imag ~= nil then
		for i = 1,n do
			for j=1,#m1.m_real.m_data do
				m.m_real.m_data[index + (i-1)*index + j] = m1.m_real.m_data[j]
				m.m_imag.m_data[index + (i-1)*index + j] = m1.m_imag.m_data[j]
			end
		end
	end
	m.n_cols = m.n_cols + n
	return m
end


--
-- 工厂函数
-- 放大多少倍
--
function nrow(m,n)
	if not m.b_init or m.m_data == nil then
		return m
	end
	m = m:t()
	m = ncol(m,n)
	m = m:t()
	return m
end


--
-- 快速获取一个系列
--
function range(start,stop,counts)
	if counts == nil then
		counts = stop -  start + 1
	end
	return linearity:range(start,stop,counts)
end


--
-- 读取带头部信息的数据
-- filename 字符串类型 数据所在的文件名
-- offset 数据在文件中的便宜大小 只有二进制文件这个数才有意义
-- bitnum 数据大小的位数 4 表示单精度浮点数 8 表示双精度浮点数
-- isbinary 是否是二进制文件
-- iscontinue 这个参数有点奇葩，bool类型 表示是否需要接着上次读取的位置继续读
-- iscontinue == true 的话仅仅适应于连续读取固定文件的内容
-- eg
-- filename = "xxx.dat"
-- m1 = load_data(filename,0,4,false,false)
-- m2 = load_data(filename,0,4,false,true)
-- m3 = load_data(filename,0,4,false,true)
-- 此时的 m1 m2 m3 是文件中连续的三块数据
-- 该接口能够读取的文件格式如下
--  二进制文件：
-- row,col,data.......
-- 文本文件:
--  row  col
--  data....
--  对于二进制文件的行列是4字节的整形
--  对于文本文件数据以空格或者制表符分隔
--
function load_data(filename,offset,bitnum,isbinary,iscontinue)
	local isok,row,col,data = LuaMatrix.load2(filename,offset,bitnum,isbinary,iscontinue)
	if not isok or row == nil or col == nil or data == nil  then
		return 
	end
	local m = matrix:new()
	m.b_init = true
	m.n_cols = col
	m.n_rows = row
	m.m_data = data
	return m
end

