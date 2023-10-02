#include <inttypes.h>

class Application
{
public:
	Application(void);

	void Initialize(void);

	void Update(void);

private:
	void SetupI2S(void);
	void I2SRoutine(void);

	static void I2SRoutine(void* arg)
	{
		reinterpret_cast<Application*>(arg)->I2SRoutine();
	}

private:
	uint32_t m_AvailableTicks;
	uint32_t m_AvailableTicksStart;
	uint32_t m_AvailableTicksEnd;

	uint32_t m_UsedTicks;
	uint32_t m_UsedTicksStart;
	uint32_t m_UsedTicksEnd;

	int32_t* m_InBufferInt;
	float* m_InBuffer;

	int32_t* m_OutBufferInt;
	float* m_OutBuffer;

	bool m_IsMuted;

	float m_OutCorrectionGain;
};