fn main() {
}

fn dna_strand(dna: &str) -> String {
    let mut result: String = "".to_string();
    for c in dna.chars() { 
        if c == 'C' {
            result += "G"
        }
        if c == 'G' {
            result += "C"
        }
        if c == 'T' {
            result += "A"
        }
        if c == 'A' {
            result += "T"
        }
    }
    return result;
}


#[cfg(test)]
mod tests {
    use super::dna_strand;

    fn dotest(s: &str, expected: &str) {
        let actual = dna_strand(s);
        assert!(actual == expected, 
            "With dna = \"{s}\"\nExpected \"{expected}\" but got \"{actual}\"")
    }
    
    #[test]
    fn fixed_tests_1() {
        dotest("AAAA","TTTT");
    }
    #[test]
    fn fixed_tests_2() {
        dotest("ATTGC","TAACG");
    }
    #[test]
    fn fixed_tests_3() {
        dotest("GTAT","CATA");
    }
    #[test]
    fn fixed_tests_4() {
        dotest("ATCG","TAGC");
    }
    #[test]
    fn fixed_tests_5() {
        dotest("GGCCAATTGCAT","CCGGTTAACGTA");
    }
}