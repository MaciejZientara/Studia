fn main() {
}

fn count_red_beads(n: u32) -> u32 {
    if n<2 {
        return 0;
    }
    (n-1)*2
}

#[cfg(test)]
mod tests {
    use super::count_red_beads;

    #[test]
    fn returns_expected_1() {
        assert_eq!(count_red_beads(0), 0);
    }
    #[test]
    fn returns_expected_2() {
        assert_eq!(count_red_beads(1), 0);
    }
    #[test]
    fn returns_expected_3() {
        assert_eq!(count_red_beads(3), 4);
    }
    #[test]
    fn returns_expected_4() {
        assert_eq!(count_red_beads(5), 8);
    }
    #[test]
    fn returns_expected_5() {
        assert_eq!(count_red_beads(5), 8);
    }
}