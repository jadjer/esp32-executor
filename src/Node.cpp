// Copyright 2023 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

//
// Created by jadjer on 25.09.23.
//

#include "executor/Node.hpp"

#include <esp_timer.h>

namespace executor {

uint32_t convertFrequencyToPeriodInUS(float const frequency) {
  auto const period_InSecond = 1 / frequency;
  auto const period_InUS = static_cast<uint32_t>(period_InSecond * 1000000);

  return period_InUS;
}

Node::Node(float const frequency) : m_lastSpinTime_InUS(0), m_updatePeriod_InUS(convertFrequencyToPeriodInUS(frequency)) {}

void Node::setFrequency(float const frequency) {
  m_updatePeriod_InUS = convertFrequencyToPeriodInUS(frequency);
}

void Node::spinOnce() {
  auto const currentTime_InUS = esp_timer_get_time();

  auto const diffTime_InUS = currentTime_InUS - m_lastSpinTime_InUS;
  if (diffTime_InUS < m_updatePeriod_InUS) {
    return;
  }

  process();

  m_lastSpinTime_InUS = currentTime_InUS;
}

}
