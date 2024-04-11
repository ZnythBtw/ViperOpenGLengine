#include "appTime.h"
#include <string>
#include <vector>
#include <algorithm>

double& VPEAppTimeManip::GetDeltaTime()
{
	static uint32_t counter = 0;
	const double finalTime{ glfwGetTime() };
	double deltaTime{ finalTime - this->zeroTime };

	counter++;
	this->timeCounter += this->DeltaTime;

	if (deltaTime > 0.0)
	{
		this->zeroTime = finalTime;
		this->fps = (1.0 / this->DeltaTime) * counter;
		counter = 0;
	}
	return deltaTime;
}

const float& VPEAppTimeManip::GetTickRateFactor(VPETickRate& TickRate)
{
	static std::vector<double> TickValues;
	static double TickFactor = this->DeltaTime;

	TickValues.push_back(this->DeltaTime);

	if (TickValues.size() == TickRate)
	{
		std::for_each(TickValues.begin(), TickValues.end(), [](const double& Tick)->void
					  {
						  TickFactor += Tick;
						  return;
					  });

		TickValues.clear();
		TickFactor /= TickRate;
	}
	return static_cast<float>(TickFactor);
}

void VPEAppTimeManip::UpdateFPSWindowTitle(GLFWwindow* window)
{
	if (this->timeCounter > this->timeCounterLimit)
	{
		glfwSetWindowTitle(window, (std::to_string(this->fps) + " Fps").c_str());
		this->timeCounter = 0.0;
	}
	return;
}