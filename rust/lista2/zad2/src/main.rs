fn main() {
    println!("Hello, world!");
}

fn summy(strng: &str) -> i32 {
    let split = strng.split(' ');
    let mut res = 0;
    for s in split {
        if !s.is_empty(){
            res += s.parse::<i32>().unwrap();
        }
    }
    return res;
}


#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn sample_tests1() {
        assert_eq!(summy("1 2 3"), 6);
    }
    #[test]
    fn sample_tests2() {
        assert_eq!(summy("1 2 3 4"), 10);
    }
    #[test]
    fn sample_tests3() {
        assert_eq!(summy("1 2 3 4 5"), 15);
    }
    #[test]
    fn sample_tests4() {
        assert_eq!(summy("10 10"), 20);
    }
    #[test]
    fn sample_tests5() {
        assert_eq!(summy("0 0"), 0);
    }
}
