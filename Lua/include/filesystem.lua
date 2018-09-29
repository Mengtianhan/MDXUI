require("LuaDir")

filesystem = {}

--递归遍历一个目录，返回所有文件的table
function filesystem:recursive_dir(dir)
	return LuaDir.TraverseDir(dir)
end

--在指定目录中查找文件
--返回值为bool 和 string
function filesystem:findfile(dir,filename)
	return LuaDir.FindFile(dir,filename)
end

--在指定目录中查找文件
--返回满足条件的所有文件的table
function filesystem:findfiles(dir,file_patten)
	return LuaDir.FindFiles(dir,file_patten)
end

--将一个目录copy到指定的位置
function filesystem:copyfiles(sorcedir,disdir)
	LuaDir.CopyFiles(sorcedir,disdir)
end

--将满足条件的文件从sorcedir中copy到disdir中
function filesystem:copyfiles(sorcedir,disdir,file_patten)
	LuaDir.CopyFiles(sorcedir,disdir,file_patten)
end

--从指定目录中移除所有文件
function filesystem:removefiles(dir)
	LuaDir.RemoveFiles(dir)
end

--从指定目录中移除和file_patten的文件
function filesystem:removefiles(dir,file_patten)
	LuaDir.RemoveFiles(dir,file_patten)
end

--检查指定文件是否存在
function filesystem:fileisexist(filename)
	return LuaDir.FileIsExist(filename)
end

--检查指定文件夹是否存在
function filesystem:folderisexist(foldername)
	return LuaDir.FolderIsExist(foldername)
end

--创建一个路径
function filesystem:createdir(dir)
	LuaDir.CreateDir(dir)
end

--获取给定路径的全路径
function filesystem:completepath(path)
	return LuaDir.CompletePath(path)
end

--获取指定路径的附路径
function filesystem:parentpath(path)
	return LuaDir.Parent(path)
end

--获取root路径
function filesystem:rootpath(path)
	return LuaDir.RootPath(path)
end

--获取root目录
function filesystem:rootdir(path)
	return LuaDir.RootDir(path)
end

--获取root名
function filesystem:rootname(path)
	return LuaDir.RootName(path)
end

--文件名
function filesystem:filename(path)
	return LuaDir.FileName(path)
end

--不带扩展名的文件名
function filesystem:left(path)
	return LuaDir.Left(path)
end

--扩展名
function filesystem:extension(path)
	return LuaDir.Extension(path)
end

--是否为全路径
function filesystem:iscompletePath(path)
	return LuaDir.IsCompletePath(path)
end

--移除文件名 剩下路径部分
function filesystem:removefilename(path)
	return LuaDir.RemoveFilename(path)
end

--替换扩展名
function filesystem:replace_ext(path,newext)
	return LuaDir.ReplaceExtension(path,newext)
end

--exe路径
function filesystem:exe_path()
	return LuaDir.ExePath()
end

--错误对话框
function filesystem:error_box(msg)
	LuaDir.ErrorBox(msg)
end


--获取当前路径
function filesystem:currentpath()
	return LuaDir.CurrentPath()
end

--获取当前系统时间
function filesystem:currenttime()
	return LuaDir.CurrentTime()
end

--获取uuid字符串
function filesystem:uuidstr()
	return LuaDir.CreateUUIDStr()
end

--获取指定文件的大小
function filesystem:filesize(filename)
	return LuaDir.FileSize(filename)
end

--修改文件名
function filesystem:renamefile(oldname,newname)
	LuaDir.Rename(oldname,newname)
end

