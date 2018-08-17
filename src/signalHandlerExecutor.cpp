/* Copyright 2018 Laurent Van Begin
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <signalHandlerExecutor.h>
#include <csignal>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>

class SignalHandlerExecutor::HandlerInfo {
public:
    int id;
    std::function<void()> handler;
    ~HandlerInfo() = default;
    static HandlerInfo *create(std::function<void()> handler) {
        return new HandlerInfo(ids++, handler);
    }

private:
    static int ids;
    HandlerInfo(int id, std::function<void()> handler) : id(id), handler(handler) { }
};

int  SignalHandlerExecutor::HandlerInfo::ids = 0;

int SignalHandlerExecutor::addHandler(int sig, std::function<void()> handler) {
    std::lock_guard<std::mutex> l(m);

    if (!isCatchable(sig))
        throw std::runtime_error("Error: uncatchable signal");
    auto handlerInfo = HandlerInfo::create(handler);
    all_handlers[sig].push_back(handlerInfo); 
    return handlerInfo->id;
}

void SignalHandlerExecutor::removeHandler(int sig, int handlerId) {
    std::lock_guard<std::mutex> l(m);

    if (!isCatchable(sig))
        throw std::runtime_error("Error: uncatchable signal");
    auto handlers = all_handlers[sig];
    auto it = std::find_if(handlers.begin(), handlers.end(), [handlerId](auto elem) { return handlerId == elem->id;});
    if (it == handlers.end())
        throw std::out_of_range("handler does not exist");
    delete *it;
    handlers.erase(it);
    all_handlers[sig] = std::move(handlers);
}

bool SignalHandlerExecutor::isCatchable(int sig) {
    auto b = uncatchableSignals.begin();
    auto e = uncatchableSignals.end();
    return e == std::find(b, e, sig);
}

void SignalHandlerExecutor::start() {
    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);
 
    std::thread([set]() {
        while (true)  {
            int sig;
            auto s = sigwait(&set, &sig);
            if (0 != s) {
                std::cerr << "sigwait failed with error " << s << std::endl;  
                exit(-1);
            }
            {
                std::lock_guard<std::mutex> l(m);
                auto handlers = all_handlers[sig];
                std::for_each(handlers.begin(), handlers.end(), [] (auto *h) {
                    h->handler();
               });
            }    
        }
    }).detach();
}

std::mutex SignalHandlerExecutor::m;
const std::vector<int> SignalHandlerExecutor::uncatchableSignals = { SIGSTOP, SIGKILL, }; 
std::map<int, std::vector<SignalHandlerExecutor::HandlerInfo *>> SignalHandlerExecutor::all_handlers;
bool SignalHandlerExecutor::dummy = (SignalHandlerExecutor::start(), 0);
