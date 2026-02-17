#define RCC_BASE   0x40023800
#define GPIOB_BASE 0x40020400
#define I2C1_BASE  0x40005400

#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x40))

#define GPIOB_MODER  (*(volatile unsigned int *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile unsigned int *)(GPIOB_BASE + 0x04))
#define GPIOB_PUPDR  (*(volatile unsigned int *)(GPIOB_BASE + 0x0C))
#define GPIOB_AFRH   (*(volatile unsigned int *)(GPIOB_BASE + 0x24))

#define I2C1_CR1   (*(volatile unsigned int *)(I2C1_BASE + 0x00))
#define I2C1_CR2   (*(volatile unsigned int *)(I2C1_BASE + 0x04))
#define I2C1_DR    (*(volatile unsigned int *)(I2C1_BASE + 0x10))
#define I2C1_SR1   (*(volatile unsigned int *)(I2C1_BASE + 0x14))
#define I2C1_SR2   (*(volatile unsigned int *)(I2C1_BASE + 0x18))
#define I2C1_CCR   (*(volatile unsigned int *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE (*(volatile unsigned int *)(I2C1_BASE + 0x20))

void I2C1_Init(void);
void I2C1_Send(char saddr, int n, char *str);

int main(void)
{
	I2C1_Init();
	while (1)
	{
		I2C1_Send(0x12, 6, "Shakil");
	}
}

void I2C1_Init(void)
{
	RCC_AHB1ENR |= (1 << 1);

	GPIOB_MODER |= (1 << 19) | (1 << 17);
	GPIOB_MODER &= ~((1 << 18) | (1 << 16));

	GPIOB_OTYPER |= (1 << 8) | (1 << 9);

	GPIOB_PUPDR |= (1 << 18) | (1 << 16);
	GPIOB_PUPDR &= ~((1 << 19) | (1 << 17));

	GPIOB_AFRH &= ~(0xFF << 0);
	GPIOB_AFRH |= (1 << 2) | (1 << 6);

	RCC_APB1ENR |= (1 << 21);

	I2C1_CR1 |= (1 << 15);
	I2C1_CR1 &= ~(1 << 15);

	I2C1_CR2 = 16;
	I2C1_CCR = 80;
	I2C1_TRISE = 17;

	I2C1_CR1 |= (1 << 0);
}

void I2C1_Send(char saddr, int n, char *str)
{
	while (I2C1_SR2 & (1 << 1)) {}

	I2C1_CR1 |= (1 << 8);
	while (!(I2C1_SR1 & (1 << 0))) {}

	I2C1_DR = (saddr << 1);
	while (!(I2C1_SR1 & (1 << 1))) {}

	(void)I2C1_SR2;

	for (int i = 0; i < n; i++)
	{
		while (!(I2C1_SR1 & (1 << 7))) {}
		I2C1_DR = *str++;
	}

	while (!(I2C1_SR1 & (1 << 2))) {}
	I2C1_CR1 |= (1 << 9);
}
