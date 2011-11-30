default:
	gcc correct.c -o correct_correct
	llvm-gcc -emit-llvm correct.c -c -o correct.bc
	opt -loop-simplify correct.bc -o correct.ls.bc
	opt -insert-edge-profiling correct.bc -o correct.profile.bc
	llc correct.profile.bc -o correct.profile.s
	g++ -o correct.profile correct.profile.s /y/students/mviscomi/llvm-install/Release/lib/profile_rt.so
	./correct.profile ../input1/cccp.c > output_profile
	 opt -load ../../../Debug+Asserts/lib/projpass.so -lamp-insts -insert-lamp-profiling -insert-lamp-loop-profiling -insert-lamp-init < correct.ls.bc > correct.lamp.bc
	llc correct.lamp.bc -o correct.lamp.s
	g++ -o correct.lamp correct.lamp.s /y/students/mviscomi/llvm-install/Release/lib/profile_rt.so ../../../tools/lamp-profiler/lamp_hooks.o ../../../tools/lamp-profiler/loadstride.cxx
	./correct.lamp ../input1/cccp.c > output
	opt -load ../../../Debug+Asserts/lib/projpass.so -profile-loader -profile-info-file=llvmprof.out -lamp-inst-cnt -lamp-map-loop -lamp-load-profile -projpass correct.ls.bc > correct.slicm.bc
	llc correct.slicm.bc -o correct.slicm.s
	g++ -o correct.slicm correct.slicm.s
	opt -mem2reg correct.slicm.bc -o correct.slicm.mem2reg.bc
	llc correct.slicm.mem2reg.bc -o correct.slicm.mem2reg.s
	g++ -o correct.slicm.mem2reg correct.slicm.mem2reg.s
	opt -dot-cfg correct.slicm.bc >& /dev/null
	 mv cfg.main.dot cfg.slicm.dot
	 opt -dot-cfg correct.bc >& /dev/null
	 mv cfg.main.dot cfg.correct.dot
	 rm -f cfg.main.dot
	 opt -dot-cfg correct.slicm.mem2reg.bc >& /dev/null
	 mv cfg.main.dot cfg.slicm.mem2reg.dot
	 rm -f cfg.main.dot
	-./correct_correct ../input1/cccp.c > output_output
	-./correct.slicm.mem2reg ../input1/cccp.c > output_slicm
	diff output_slicm output_output
	 
clean:
	 rm -f llvmprof.out
	 rm -f benchmark.*
	 rm -f result.lamp.*
	 rm -f correct.bc correct.lamp* correct.ls* correct.profile*
	 rm -f output* output.lamp
	 rm -f correct_correct
	 rm -f correct.slicm*
	 rm -f cfg*

