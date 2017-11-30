/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * Generic AST walker.
 */

#pragma once

#include <libjulia/Aliases.h>

#include <libsolidity/interface/Exceptions.h>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <vector>
#include <set>
#include <map>

namespace dev
{
namespace julia
{

/**
 * Generic AST walker.
 */
class ASTWalker: public boost::static_visitor<>
{
public:
	virtual void operator()(Literal const&) {}
	virtual void operator()(Instruction const&) { solAssert(false, ""); }
	virtual void operator()(Identifier const&) {}
	virtual void operator()(FunctionalInstruction const& _instr);
	virtual void operator()(FunctionCall const& _funCall);
	virtual void operator()(Label const&) { solAssert(false, ""); }
	virtual void operator()(StackAssignment const&) { solAssert(false, ""); }
	virtual void operator()(Assignment const& _assignment);
	virtual void operator()(VariableDeclaration const& _varDecl);
	virtual void operator()(If const& _if);
	virtual void operator()(Switch const& _switch);
	virtual void operator()(FunctionDefinition const&);
	virtual void operator()(ForLoop const&);
	virtual void operator()(Block const& _block);

protected:
	template <class T>
	void walkVector(T const& _statements)
	{
		for (auto const& st: _statements)
			boost::apply_visitor(*this, st);
	}
};

/**
 * Specific AST walker that collects all defined names.
 */
class NameCollector: public ASTWalker
{
public:
	using ASTWalker::operator ();
	virtual void operator()(VariableDeclaration const& _varDecl) override;
	virtual void operator()(FunctionDefinition const& _funDef) override;

	std::set<std::string> const& names() const { return m_names; }
	std::map<std::string, FunctionDefinition const*> const& functions() const { return m_functions; }
private:
	std::set<std::string> m_names;
	std::map<std::string, FunctionDefinition const*> m_functions;
};

}
}