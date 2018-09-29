require("include.matrix")


fft = {}


--
-- 矩阵相乘
-- 适用于复数矩阵的相乘
-- 返回的结果是一个复数矩阵
-- 如果输入参数都是实数矩阵，那么返回结果的虚部全为0
--
function fft.mmult(m1,m2)
	if not m1.b_init or not m2.b_init then
		print("Error:not m1.b_init or not m2.b_init",m1.b_init,m2.b_init)
		return
	end
	
	--
	-- 手写矩阵的乘法实在是太慢
	-- 直接使用C++实现的版本进行矩阵的乘法
	--
	if true then
		return m1*m2
	end
	
	--
	-- 下面是lua计算矩阵的乘法
	-- 如果不满足矩阵的乘法原则直接返回
	--
	if m1.n_cols ~= m2.n_rows then
		print("m1.n_cols ~= m2.n_rows")
		return
	end
	
	local real = zeros(m1.n_rows,m2.n_cols)
	local imag = zeros(m1.n_rows,m2.n_cols)
		
	if m1.m_data ~= nil and m2.m_data ~= nil then
		--
		-- 如果两个矩阵都是实数矩阵
		--	
		
		for i=1,m1.n_rows do
			
			for j=1,m2.n_cols do
				local val = 0
				--
				-- m1 的 列 == m2 的行
				--
				for k = 1,m2.n_rows do
					val = val + m1:cell(i,k)*m2:cell(k,j)
				end
				
				real:cell(i,j,val)
			end
		end
		return complex(real,imag)
		--
		--------------------------
		--
	elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_data ~= nil then
		--
		-- 如果变换矩阵是复矩阵 原数据为实矩阵
		--
		for i=1,m1.n_rows do
			
			for j=1,m2.n_cols do
				local val1 = 0
				local val2 = 0
				--
				-- m1 的 列 == m2 的行
				--
				for k = 1,m2.n_rows do
					local r1,i1 = m1:cell(i,k)
					local r0 = m2:cell(k,j)
					val1 = val1 + r1*r0
					val2 = val2 + i1*r0
				end
				
				real:cell(i,j,val1)
				imag:cell(i,j,val2)
			end
		end
		return complex(real,imag)
	
	elseif m1.m_real ~= nil and m1.m_imag ~= nil and m2.m_real ~= nil and m2.m_imag ~= nil then
		--
		-- 如果两者都是复数
		--
		for i=1,m1.n_rows do
			
			for j=1,m2.n_cols do
				local val1 = 0
				local val2 = 0
				--
				-- m1 的 列 == m2 的行
				--
				for k = 1,m2.n_rows do
					local r1,i1 = m1:cell(i,k)
					local r2,i2 = m2:cell(k,j)
					val1 = val1 + r1*r2 - i1*i2
					val2 = val2 + i1*r2 + r1*i2
				end
				
				real:cell(i,j,val1)
				imag:cell(i,j,val2)
			end
		end
		return complex(real,imag)
	end
	print("Error : Type Error")
end


--
-- 一维傅里叶变换
-- 该方法使用的是傅里叶矩阵和变换对象相乘的方法
-- 速度较慢
-- 待研究完成矩阵的分块后可以将时间复杂度提升到n/2*log2(n)
-- 在matrix里面的linearity里有快速的fft变换
--
function fft.fft(m,num)
	if not m.b_init or m.m_data == nil then
		return
	end
	local t = m:detach()
	local k = math.log(num,2)
	local n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	
	num = 2^n
	
	--
	-- 变换大小为2的n次幂
	--
	if #t < num then
		local diff = num - #t
		local index = #t
		for i=1,diff do
			t[index + i] = 0
		end
	end
	
	--
	-- 创建傅里叶矩阵
	--
	local fm = fftm(num)
	local org = mat()
	org:attach(t,num,1)
	return fft.mmult(fm,org)
end


--
-- 一维傅里叶逆变换
-- 该方法使用的是傅里叶矩阵和变换对象相乘的方法
-- 速度较慢
-- 待研究完成矩阵的分块后可以将时间复杂度提升到n/2*log2(n)
-- 在matrix里面的linearity里有快速的fft变换
--
function fft.ifft(m,num)
	if not m.b_init or m.m_data == nil then
		return
	end
	local t = m:detach()
	local k = math.log(num,2)
	local n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	
	num = 2^n
	
	--
	-- 变换大小为2的n次幂
	--
	if #t < num then
		local diff = num - #t
		local index = #t
		for i=1,diff do
			t[index + i] = 0
		end
	end
	
	--
	-- 创建傅里叶矩阵
	--
	local fm = ifftm(num)
	local org = mat()
	org:attach(t,num,1)
	return fft.mmult(fm,org)
end

--
-- 二维离散福利变换
-- 正变换
--
function fft.fft2(m,rownum,colnum)
	if not m.b_init then
		return
	end
	local k = math.log(rownum,2)
	local n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	rownum = 2^n
	
	k = math.log(colnum,2)
	n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	colnum = 2^n
	
	local tempm = m:clone()
	tempm:resize(rownum,colnum)
	
	--
	-- 针对行的变换矩阵
	--
	local fm = fftm(colnum)

	
	--
	-- 对行进行变换
	--
	local colm = tempm:t()
	colm = fft.mmult(fm,colm)
	
	--
	-- 针对列的变换矩阵
	--
	colm = colm:t()
	fm = fftm(rownum)
	return fft.mmult(fm,colm)
end

--
-- 二维离散傅里叶逆变换
-- 点数不能太多了
--
function fft.ifft2(m,rownum,colnum)
	if not m.b_init then
		return
	end
	local k = math.log(rownum,2)
	local n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	rownum = 2^n
	
	k = math.log(colnum,2)
	n,r = math.modf(k)
	if r > 0 then
		n = n + 1
	end
	colnum = 2^n
	
	local tempm = m:clone()
	tempm:resize(rownum,colnum)
	
	--
	-- 针对行的变换矩阵
	--
	local fm = ifftm(colnum)

	--
	-- 对行进行变换
	--
	local colm = tempm:t()
	colm = fft.mmult(fm,colm)
	
	--
	-- 针对列的变换矩阵
	--
	colm = colm:t()
	fm = ifftm(rownum)
	return fft.mmult(fm,colm)
end

