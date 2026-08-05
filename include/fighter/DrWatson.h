#ifndef WATSON_H
#define WATSON_H

#include "Sidekick.h"

class Watson : public Sidekick
{
    public:
        Watson();

        ~Watson() override = default;

        bool isAbilityImmune() const override;
};

#endif