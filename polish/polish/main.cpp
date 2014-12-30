#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <map>

#include <boost/lexical_cast.hpp>

typedef std::string str_t;
typedef std::vector< str_t > vec_str;
typedef std::map< str_t,  double > vars_map;
typedef std::map< str_t, vec_str::iterator > iter_map;
typedef std::map< str_t, int > binops_map;

int main(int argc, char* argv[])
{
    vars_map vars;
    vec_str vec;
    iter_map label;
    binops_map binops;

    binops.emplace(std::make_pair("+" ,  1));
    binops.emplace(std::make_pair("-" ,  2));
    binops.emplace(std::make_pair("*" ,  3));
    binops.emplace(std::make_pair("/" ,  4));

    binops.emplace(std::make_pair("<" ,  5));
    binops.emplace(std::make_pair("<=",  6));
    binops.emplace(std::make_pair(">" ,  7));
    binops.emplace(std::make_pair(">=",  8));
    binops.emplace(std::make_pair("==",  9));
    binops.emplace(std::make_pair("!=", 10));

    {
        char** p = argv + 1;

        while (std::strcmp(*p, "|"))
            vars[*p++];
        vec.swap(vec_str(++p, argv + argc));
    }
    
    {
        for (auto it = vec.begin(), end = vec.end(); it != end; ++it)
        {
            if ((*it)[0] == '_' && (it + 1) == end)
                label[*it] = it;
            else if ((*it)[0] == '_' && (it + 1) != end && *(it + 1) != "JIF" && *(it + 1) != "JMP")
                label[*it] = it;
        }
    }

    std::stack<std::pair< str_t, bool >> stack;
    std::stack<bool> bool_stack;

    for (auto it = vec.begin(), end = vec.end(); it != end;)
    {
        if (*it == "READ")
        {
            int amount = boost::lexical_cast<int>(stack.top().first);
            stack.pop();
            auto i = it - amount - 1;
            while (amount--)
            {
                std::cin >> vars[*i];
                std::cin.ignore();
                stack.pop();
                ++i;
            }
        }
        else if (*it == "WRITE")
        {
            int amount = boost::lexical_cast<int>(stack.top().first);
            stack.pop();
            std::stack<std::pair< str_t, bool >> tmp;
            while (amount--)
            {
                tmp.push(stack.top());
                stack.pop();
            }
            while (!tmp.empty())
            {
                if (tmp.top().second == false)
                    std::cout << vars[tmp.top().first];
                else if (tmp.top().first[0] == '\"')
                {
                    str_t& s = tmp.top().first;
                    s.erase(s.begin());
                    s.erase(s.end() - 1);
                    std::cout << s;
                }
                else
                    std::cout << tmp.top().first;
                tmp.pop();
            }
            std::cout << std::endl;
        }
        else if (*it == "JIF")
        {
            if (bool_stack.top() == false)
                it = label[stack.top().first];
            bool_stack.pop();
            stack.pop();
            ++it;
            continue;
        }
        else if (*it == "JMP")
        {
            it = label[stack.top().first] + 1;
            stack.pop();
            continue;
        }
        else if ((*it)[0] == '_') //label
        {
            stack.push(std::make_pair(*it, true));
        }
        else if (*it == "@")
        {
            if (stack.top().second == true)
            {
                str_t& s = stack.top().first;
                s.insert(s.begin(), 1, '-');
            }
            else
            {
                double& val = vars[stack.top().first];
                val *= -1;
            }
        }
        else if (*it == "!")
        {
            bool_stack.top() = !bool_stack.top();
        }
        else if (*it == "||")
        {
            bool op2 = bool_stack.top();
            bool_stack.pop();
            bool op1 = bool_stack.top();
            bool_stack.pop();
            bool_stack.push(op1 || op2);
        }
        else if (*it == "&&")
        {
            bool op2 = bool_stack.top();
            bool_stack.pop();
            bool op1 = bool_stack.top();
            bool_stack.pop();
            bool_stack.push(op1 && op2);
        }
        else if (*it == "=")
        {
            auto p2 = stack.top();
            stack.pop();
            auto p1 = stack.top();
            stack.pop();
            vars[p1.first] = (p2.second == true ? boost::lexical_cast<double>(p2.first) : vars[p2.first]);
        }
        else if (vars.find(*it) != vars.end()) //variables
        {
            stack.push(std::make_pair(*it, false));
        }
        else if ((*it)[0] == '.' || std::isdigit((*it)[0])) //numbers
        {
            stack.push(std::make_pair(*it, true));
        }
        else if (binops.find(*it) != binops.end())
        {
            int oper = binops.find(*it)->second;
            auto p2 = stack.top();
            stack.pop();
            double op2 = (p2.second == true ? boost::lexical_cast<double>(p2.first) : vars[p2.first]);
            auto p1 = stack.top();
            stack.pop();
            double op1 = (p1.second == true ? boost::lexical_cast<double>(p1.first) : vars[p1.first]);

            switch (oper)
            {
            case  1: op1 += op2; break;
            case  2: op1 -= op2; break;
            case  3: op1 *= op2; break;
            case  4: 
                assert(op2 != 0 && "Division by zero");
                op1 /= op2; 
                break; //fix me: division by zero
            case  5: bool_stack.push(op1 <  op2); break;
            case  6: bool_stack.push(op1 <= op2); break;
            case  7: bool_stack.push(op1 >  op2); break;
            case  8: bool_stack.push(op1 >= op2); break;
            case  9: bool_stack.push(op1 == op2); break;
            case 10: bool_stack.push(op1 != op2); break;
            }
            if (oper < 5)
                stack.push(std::make_pair(boost::lexical_cast<std::string>(op1), true));
        }
        else //string
        {
            stack.push(std::make_pair(*it, true));
        }
        ++it;
    }

/* DEBUG
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<str_t>(std::cout, "\n"));
    std::system("pause");
    for (auto it = vars.begin(); it != vars.end(); ++it)
        std::cout << it->first << '\n';
    std::system("pause");
    for (auto it = label.begin(); it != label.end(); ++it)
        std::cout << it->first << '\n';
*/

    std::system("pause");
    return 0;
}