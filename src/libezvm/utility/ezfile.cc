#include "ezvm/ezfile.h"
#include <cerrno>
#include <cstring>
#include <cstdarg>
#include <stdexcept>
ezFile::ezFile():m_fd(NULL) {
}

ezFile::ezFile(const string path, const string mode):m_fd(NULL) {
	if(path == "stdout") {
		m_fd = stdout;
	} else if(path == "stderr") {
		m_fd = stderr;
	} else if(!path.empty()) {
		if(mode.empty()) throw runtime_error("mode is empty");
		m_fd = fopen(path.c_str(), mode.c_str());
		if(!m_fd) throw runtime_error(strerror(errno));
	}
}

ezFile::~ezFile() {
	if(!m_fd || m_fd == stdout || m_fd == stderr) return;
	fclose(m_fd);
}

void ezFile::println(const char* fmt, ...){
	if(!m_fd) return;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(m_fd, fmt, ap);
	va_end(ap);
	fprintf(m_fd, "\n");
}

void ezFile::vprintln(const char* fmt, va_list ap) { 
	vfprintf(m_fd, fmt, ap);
	fprintf(m_fd, "\n");
}

void ezFile::print(const char* fmt, ...) {
	if(!m_fd) return;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(m_fd, fmt, ap);
	va_end(ap);
}

void ezFile::vprint(const char* fmt, va_list ap) { 
	vfprintf(m_fd, fmt, ap);
}

