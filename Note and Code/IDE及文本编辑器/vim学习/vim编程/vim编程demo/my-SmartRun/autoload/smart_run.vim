"一键运行
function smart_run#SmartCompile()
    exec "wa" 
    if &filetype == "c"
        exec "AsyncRun mkdir -p bin && gcc -Wall -g -std=c99 $(VIM_FILEPATH) -Iinclude -lpthread -o $(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out "
    elseif (&filetype == "cpp") "cc cpp后缀均适用
        "exec ""AsyncRun mkdir -p bin && g++ -DCOMMLIB_MINI_VIDEO_PACKET_VER=100 -DCOMMLIB_VIDEO_PACKET_VER=410 -DSPP_RPC_CPP_VERSION=201103 -DSPP_RPC_SHORT_LOG_FILE_NAME=1 -DSPP_RPC_VERSION=1  -O0 -DSPP_RPC_DEBUG=1 -DSPP_RPC_FRAME_DEBUG=1 -DSPP_RPC_API_DEBUG=1 -I/usr/local/commlib/platformlib/spp_rpc_sumeru/common/googletest/include -I/usr/local/commlib/platformlib/spp_rpc_sumeru/common/tarsutil/include -L/usr/local/commlib/platformlib/spp_rpc_sumeru/common/tarsutil/lib -L/usr/local/commlib/platformlib/spp_rpc_sumeru/common/googletest/lib64 -g -std=c++11 -Wno-literal-suffix  -DSPP_RPC_FRAME_FLAG=1 -fPIC -Wall -ltarsutil -lgtest -ldl -lz -lpthread -lrt -m64 -Wno-deprecated -o $(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out $(VIM_FILENAME)"
        "exec ""AsyncRun mkdir -p bin && g++ -I/usr/local/commlib/outerlib/googletest-1.8.1/include -I/usr/local/commlib/platformlib/spp_rpc_sumeru/common/tarsutil/include -L/usr/local/commlib/platformlib/spp_rpc_sumeru/common/tarsutil/lib -L/usr/local/commlib/outerlib/googletest-1.8.1/lib -g -std=c++11 -ltarsutil -lgtest -ldl -lz -lrt -lpthread -o $(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out $(VIM_FILENAME)"
        exec "AsyncRun mkdir -p bin && g++ $(VIM_FILEPATH) -g -O0 -std=c++11 -I/usr/local/commlib/platformlib/spp_rpc_sumeru/common/googletest/include -L/usr/local/commlib/platformlib/spp_rpc_sumeru/common/googletest/lib64 -lgtest -lpthread -o $(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out "
    elseif (&filetype == "go")
        exec "AsyncRun mkdir -p $(VIM_FILEDIR)/bin && go build -o $(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out $(VIM_FILENAME)"
    elseif (&filetype == "python")
        exec "AsyncRun -raw python3 $(VIM_FILEPATH)"
    else
        echom "Error! file type (".&filetype.") could not be compiled"
    endif
endfunction

