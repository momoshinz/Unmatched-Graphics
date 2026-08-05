#ifndef SISTERS_H
#define SISTERS_H
#include "Sidekick.h"
#include <string>
using namespace std;

class Sisters : public Sidekick
{
    private: 
        int id;

    public:
        Sisters(int id);
        ~Sisters() override = default;
        int getID() const;
        bool isSister() const override;
};
#endif