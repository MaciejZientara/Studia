mod complex;
mod picture;

fn main() {
    println!("Hello, world!");
    let func = |z:&complex::Complex| -> complex::Complex 
    {z.pow(5)-z.pow(3)+complex::Complex::new(0.15,0.35)};
    let pic = picture::Picture::new("picture.ppm",256,256,func);
    pic.print();
}

#[cfg(test)]
mod tests {
    use super::*;
    
    fn copmare_complex(a : &complex::Complex,b : &complex::Complex) -> bool{
        let epsilon : f32 = 0.00001;
        (a.r-b.r<epsilon) && (a.i-b.i<epsilon)
    }

    #[test]
    fn complex_test_add() {
        let z = complex::Complex{r:0.5,i:2.0};
        let c = complex::Complex::new(1.0,4.0);
        assert!(copmare_complex(&(z+c), &complex::Complex{r: 1.5, i: 6.0}))
    }
    #[test]
    fn complex_test_sub() {
        let z = complex::Complex{r:0.5,i:2.0};
        let c = complex::Complex::new(1.0,4.0);
        assert!(copmare_complex(&(z-c), &complex::Complex{r: -0.5, i: -2.0}))
    }
    #[test]
    fn complex_test_mul() {
        let z = complex::Complex{r:0.5,i:2.0};
        let c = complex::Complex::new(1.0,4.0);
        assert!(copmare_complex(&(z*c), &complex::Complex{r: -7.5, i: 4.0}))
    }
    #[test]
    fn complex_test_abs() {
        let z = complex::Complex{r:0.5,i:2.0};
        let epsilon : f32 = 0.00001;
        assert!(z.abs()-2.06155<epsilon);
    }

}