mod complex;
mod picture;

fn main() {
    let func1 = |z:&complex::Complex| -> complex::Complex 
    {z.pow(7)-z.pow(5)+z.pow(3)+complex::Complex::new(0.15,0.35)};
    let pic1 = picture::Picture::new("picture1.ppm",1024,1024,func1);
    pic1.print();
    let func2 = |z:&complex::Complex| -> complex::Complex 
    {z.pow(7)-z.pow(5)+z.pow(3)-z.pow(1)+complex::Complex::new(0.15,0.35)};
    let pic2 = picture::Picture::new_single("picture2.ppm",2048,2048,func2);
    pic2.print();
}

/* 
BUT WHY???

time on new() + new_single():

real    0m34,125s
user    0m24,868s
sys     0m21,150s

time on single thread new_single():

real    0m27,605s
user    0m20,500s
sys     0m7,241s

time on multithreading new():

real    1m2,776s
user    0m43,884s
sys     1m26,730s
*/

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