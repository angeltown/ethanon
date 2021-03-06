/*--------------------------------------------------------------------------------------
 Ethanon Engine (C) Copyright 2008-2012 Andre Santee
 http://www.asantee.net/ethanon/

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------------*/

#ifndef ETH_ENTITY_CHOOSER_H_
#define ETH_ENTITY_CHOOSER_H_

#include "ETHEntity.h"
#include <vector>

class ETHEntityChooser
{
public:
	virtual bool Choose(ETHEntity* entity) const = 0;
};

class ETHEntityDefaultChooser : public ETHEntityChooser
{
public:
	bool Choose(ETHEntity* entity) const;
};

class ETHEntityNameChooser : public ETHEntityChooser
{
	gs2d::str_type::string m_name;
public:
	ETHEntityNameChooser(const gs2d::str_type::string& name);
	bool Choose(ETHEntity* entity) const;
};

class ETHEntityNameArrayChooser : public ETHEntityChooser
{
	std::vector<gs2d::str_type::string> m_names;
	bool m_isIgnoreList;
public:
	ETHEntityNameArrayChooser(const std::vector<gs2d::str_type::string>& names, const bool isIgnoreList);
	ETHEntityNameArrayChooser(const gs2d::str_type::string& semicolonSeparatedNames, const bool isIgnoreList);
	bool Choose(ETHEntity* entity) const;
};

class ETHEntitySingleExceptionChooser : public ETHEntityChooser
{
	int m_id;
public:
	ETHEntitySingleExceptionChooser(const int id);
	bool Choose(ETHEntity* entity) const;
};

#endif
