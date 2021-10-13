#include "Tools.hpp"

std::string	cut_line(std::string &str, bool cond, int back)
{
	std::string ret;

	ret = str.substr(0, str.find('\n') + back);
	if (cond == true)
		str.erase(0, str.find('\n') + 1);
	return (ret);
}


std::string	MIME_types(std::string	&str)
{
	std::string ret;
	std::map<std::string, std::string> mimes;

	ret = str.substr(str.find('.'));

	mimes[".aac"] = "audio/aac";
	mimes[".abw"] = "application/x-abiword";
	mimes[".arc"] = "application/x-freearc";
	mimes[".avi"] = "video/x-msvideo";
	mimes[".bin"] = "application/octet-stream";
	mimes[".bmp"] = "image/bmp";
	mimes[".bz"] = "application/x-bzip";
	mimes[".csh"] = "application/x-csh";
	mimes[".css"] = "text/css";
	mimes[".csv"] = "text/csv";
	mimes[".doc"] = "application/msword";
	mimes[".gz"] = "application/gzip";
	mimes[".ico"] = "image/vnd.microsoft.icon";
	mimes[".jpeg"] = "image/jpeg";
	mimes[".jpg"] = "image/jpeg";
	mimes[".html"] = "text/html";
	mimes[".htm"] = "text/html";
	mimes[".pdf"] = "application/pdf";
	mimes[".php"] = "application/x-httpd-php";
	mimes[".sh"] = "application/x-sh";
	mimes[".tar"] = "application/x-tar";
	mimes[".txt"] = "text/plain";
	mimes[".zip"] = "application/zip";

	return (mimes[ret]);
}

std::string	Last_modified(std::string &path)
{
	struct stat	ret;

	if (stat(path.c_str(), &ret) == 0)
		return (ctime(&ret.TIME.tv_sec));
	return ("");
}

std::string	status_code(int code)
{
	switch (code)
	{
	case 200:
		return ("200 OK");
		break;
	case 201:
		return ("201 Created");
		break;
	case 204:
		return ("204 No Content");
		break;
	case 400:
		return ("400 Bad Request");
		break;
	case 403:
		return ("403 Forbidden");
		break;
	case 404:
		return ("404 Not Found");
		break;
	case 405:
		return ("405 Method Not Allowed");
		break;
	case 413:
		return ("413 Payload Too Large");
		break;
	case 500:
		return ("500 Internal Server Error");
		break;
	case 501:
		return ("501 Not Implemented");
		break;
	}
	return "";
}