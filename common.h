#pragma once

#pragma warning(disable : 4819)
#include <opencv2\opencv.hpp>
#pragma warning(default : 4819)

#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib")
#else
#pragma comment(lib,"opencv_world310.lib")
#endif // _DEBUG