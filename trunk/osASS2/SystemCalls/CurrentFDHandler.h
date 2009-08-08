#ifndef CURRENTFDHANDLER_H_
#define CURRENTFDHANDLER_H_

#include <vector>
#include <algorithm>

class CurrentFDHandler
{
private:
	int _currFD;
	vector<int> _reusableFD;

public:
	CurrentFDHandler():_currFD(0){}

	int getUnusedFD()
	{
		if (_reusableFD.empty())
		{
			_currFD++;
			return _currFD;
		}
		else
		{
			int ans = _reusableFD[0];
			_reusableFD.erase(_reusableFD.begin());
			return ans;
		}
	}

	void freeFD(int fd)
	{
		_reusableFD.push_back(fd);
		sort(_reusableFD.begin(),_reusableFD.end());
	}

};


#endif
