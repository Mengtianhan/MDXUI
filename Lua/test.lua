require("include.AlgNF")
require("include.matrix")
require("window")

init()

function finished(ch,trace,freq,beam)
	print("Finished Status : ",ch,trace,freq,beam)
end

function algProgress(index,total)
	--print("Finished : ",index/total)
end

function  NfAlg()
	local alg = Alg:NFTransform("D:/Custom/Project/new_xiu/new_xiu.ltpro")
	alg:Event("FinishedStatued","finished")
	alg:Event("Progress","algProgress")
	alg:SetSaveDataFoldor("data/FFT")
	alg:SetFFTPoints(512,512)
	return alg
end


inxs = range(-90,90,181):detach()
inys = range(-90,90,181):detach()


function beam25(alg)
	return alg:AlgMid2FF("CH1","B1/A1",16E+9,25,inxs,inys,1,0,false)
end

function beampoint(alg)
	return alg:AlgBeamPoint("CH1","B1/A1",16E+9,25,0.01)
end



