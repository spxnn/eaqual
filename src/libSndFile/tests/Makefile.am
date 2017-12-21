noinst_PROGRAMS = sfversion read_seek_test floating_point_test write_read_test \
	lossy_comp_test error_test ulaw_test alaw_test sftest peak_chunk_test \
	command_test stdin_test stdout_test stdio_test 
	
SNDFILEDIR =../src
INCLUDES = -I$(srcdir)/$(SNDFILEDIR)
noinst_HEADERS = check_log_buffer.h

sfversion_SOURCES = sfversion.c 
sfversion_LDADD = $(SNDFILEDIR)/libsndfile.la

read_seek_test_SOURCES = read_seek_test.c check_log_buffer.c 
read_seek_test_LDADD = $(SNDFILEDIR)/libsndfile.la

write_read_test_SOURCES = write_read_test.c check_log_buffer.c
write_read_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

lossy_comp_test_SOURCES = lossy_comp_test.c check_log_buffer.c 
lossy_comp_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

error_test_SOURCES = error_test.c 
error_test_LDADD = $(SNDFILEDIR)/libsndfile.la

ulaw_test_SOURCES = ulaw_test.c check_log_buffer.c 
ulaw_test_LDADD = $(SNDFILEDIR)/libsndfile.la

alaw_test_SOURCES = alaw_test.c check_log_buffer.c 
alaw_test_LDADD = $(SNDFILEDIR)/libsndfile.la

floating_point_test_SOURCES = floating_point_test.c check_log_buffer.c 
floating_point_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

peak_chunk_test_SOURCES = peak_chunk_test.c 
peak_chunk_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

command_test_SOURCES = command_test.c 
command_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

sftest_SOURCES = sftest.c 
sftest_LDADD = $(SNDFILEDIR)/libsndfile.la

stdin_test_SOURCES = stdin_test.c 
stdin_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

stdout_test_SOURCES = stdout_test.c 
stdout_test_LDADD = $(SNDFILEDIR)/libsndfile.la -lm

stdio_test_SOURCES = stdio_test.c 
stdio_test_LDADD = 

check: generic-test ulaw_test alaw_test floating_point_test first-test \
		wav-tests aiff-tests au-tests raw-tests paf-tests svx-tests nist-tests \
		ircam-tests stdio-tests

generic-test : error_test ulaw_test alaw_test floating_point_test command_test
	./error_test
	./ulaw_test
	./alaw_test
	./floating_point_test
	./command_test fnorm
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed common tests."
	@echo "----------------------------------------------------------------------"

first-test: read_seek_test
	./read_seek_test wav
	./read_seek_test aiff
	./read_seek_test au
	./read_seek_test svx
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed first set of tests."
	@echo "----------------------------------------------------------------------"

wav-tests: write_read_test
	./write_read_test wav
	./lossy_comp_test wav_ima
	./lossy_comp_test wav_msadpcm
	./lossy_comp_test wav_ulaw
	./lossy_comp_test wav_alaw
	./lossy_comp_test wav_gsm610
	./peak_chunk_test wav
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on WAV files."
	@echo "----------------------------------------------------------------------"

aiff-tests: write_read_test
	./write_read_test aiff
	./peak_chunk_test aiff
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on AIFF files."
	@echo "----------------------------------------------------------------------"

au-tests: write_read_test
	./write_read_test au
	./write_read_test aule
	./lossy_comp_test au_ulaw
	./lossy_comp_test au_alaw
	./lossy_comp_test aule_ulaw
	./lossy_comp_test aule_alaw
	./lossy_comp_test au_g721
	./lossy_comp_test au_g723
	./lossy_comp_test aule_g721
	./lossy_comp_test aule_g723
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on AU files."
	@echo "----------------------------------------------------------------------"

raw-tests: write_read_test
	./write_read_test raw
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on RAW (header-less PCM) files."
	@echo "----------------------------------------------------------------------"

paf-tests: write_read_test
	./write_read_test paf
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on PAF files."
	@echo "----------------------------------------------------------------------"

svx-tests: write_read_test
	./write_read_test svx
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on SVX files."
	@echo "----------------------------------------------------------------------"

nist-tests: write_read_test
	./write_read_test nist
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on NIST files."
	@echo "----------------------------------------------------------------------"

ircam-tests: write_read_test
	./write_read_test ircam
	./lossy_comp_test ircam_ulaw
	./lossy_comp_test ircam_alaw
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed tests on IRCAM files."
	@echo "----------------------------------------------------------------------"

stdio-tests: stdio_test stdin_test stdout_test
	./stdio_test
	@echo "----------------------------------------------------------------------"
	@echo "  `./sfversion` passed stdin/stdout tests."
	@echo "----------------------------------------------------------------------"

