//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads, c++98, c++03

// <thread>

// class thread

// thread& operator=(thread&& t);

#include <thread>
#include <cassert>

class G
{
    int alive_;
public:
    static int n_alive;
    static bool op_run;

    G() : alive_(1) {++n_alive;}
    G(const G& g) : alive_(g.alive_) {++n_alive;}
    ~G() {alive_ = 0; --n_alive;}

    void operator()(int i, double j)
    {
        assert(alive_ == 1);
        assert(n_alive >= 1);
        assert(i == 5);
        assert(j == 5.5);
        op_run = true;
    }
};

int G::n_alive = 0;
bool G::op_run = false;

int main(int, char**)
{
    {
        assert(G::n_alive == 0);
        assert(!G::op_run);
        {
        G g;
        std::thread t0(g, 5, 5.5);
        std::thread::id id = t0.get_id();
        std::thread t1;
        t1 = std::move(t0);
        assert(t1.get_id() == id);
        assert(t0.get_id() == std::thread::id());
        t1.join();
        }
        assert(G::n_alive == 0);
        assert(G::op_run);
    }

  return 0;
}
