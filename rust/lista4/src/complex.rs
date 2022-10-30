use std::ops;

#[derive(Debug, Clone, Copy)]
pub struct Complex {
    pub r: f32, // real
    pub i: f32 // imagine
}

impl Complex {
    pub fn new(r: f32, i: f32) -> Complex {
        Complex{r,i}
    }

    pub fn abs(&self) -> f32 {
        (self.r * self.r + self.i * self.i).sqrt()
    }

    pub fn pow(self, power:i32) -> Complex {
        let mut res = self.clone();
        for _i in 0..power {
            res = res * self.clone();
        }
        res
    }
}

impl ops::Add<Complex> for Complex {
    type Output = Complex;

    fn add(self, rhs: Complex) -> Self::Output {
        Complex{r:self.r+rhs.r,i:self.i+rhs.i}
    }
}

impl ops::Sub<Complex> for Complex {
    type Output = Complex;

    fn sub(self, rhs: Complex) -> Self::Output {
        Complex{r:self.r-rhs.r,i:self.i-rhs.i}
    }
}

impl ops::Mul<Complex> for Complex {
    type Output = Complex;
    fn mul(self, rhs: Complex) -> Self::Output {
        Complex{r : (self.r*rhs.r)-(self.i*rhs.i),
                i : (self.r*rhs.i)+(self.i*rhs.r)}
    }
}
