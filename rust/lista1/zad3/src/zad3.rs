fn main() {
}

fn square_area_to_circle(size:f64) -> f64 {
    std::f64::consts::PI*size/4.0
    // let r = size.sqrt()/2.0;
    // std::f64::consts::PI*r*r
}

fn assert_close(a:f64, b:f64, epsilon:f64) {
    assert!( (a-b).abs() < epsilon, "Expected: {}, got: {}",b,a);
}

#[cfg(test)]
mod tests {
    use super::assert_close;
    use super::square_area_to_circle;

    #[test]
    fn returns_expected_1() {
        assert_close(square_area_to_circle(9.0), 7.0685834705770345, 1e-8);
    }
    #[test]
    fn returns_expected_2() {
        assert_close(square_area_to_circle(20.0), 15.70796326794897, 1e-8);
    }
    #[test]
    fn returns_expected_3() {
        assert_close(square_area_to_circle(30.0), 23.561944901923447, 1e-8);
    }
    #[test]
    fn returns_expected_4() {
        assert_close(square_area_to_circle(17.0), 13.351768777756622, 1e-8);
    }
    #[test]
    fn returns_expected_5() {
        assert_close(square_area_to_circle(1.0), 0.7853981633974483, 1e-8);
    }
}