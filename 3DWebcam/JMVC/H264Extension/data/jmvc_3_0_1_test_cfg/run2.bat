H264AVCEncoderLibTestStatic.exe -vf ballroom_qp32.cfg 0  >> log_0.txt
H264AVCEncoderLibTestStatic.exe -vf ballroom_qp32.cfg 2  >> log_2.txt
H264AVCEncoderLibTestStatic.exe -vf ballroom_qp32.cfg 1  >> log_1.txt

MVCBitStreamAssemblerStatic.exe -vf assembler.cfg  >> log_assembler.txt

MVCBitStreamExtractorStatic.exe ballroom.264 view0.264 -op 0 >> test.txt

H264AVCDecoderLibTestStatic.exe view0.264 view0.yuv 1 >> test.txt

H264AVCDecoderLibTestStatic.exe ballroom.264 ballroom.yuv 3 >> log_decoder.txt


