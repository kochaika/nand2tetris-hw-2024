use crate::Assembler;

fn test_file(name: &str) {
    let mut translator = Assembler::new();
    let code = std::fs::read_to_string(format!("06/{}.asm", name)).unwrap();
    let translated = translator.translate(&code).unwrap();
    let expected = std::fs::read_to_string(format!("06/{}.expected", name)).unwrap();
    assert_eq!(translated.trim().to_string(), expected.trim().to_string());
}

#[test]
fn add() {
    test_file("Add");
}

#[test]
fn max() {
    test_file("Max");
}

#[test]
fn max_l() {
    test_file("MaxL");
}

#[test]
fn pong() {
    test_file("Pong");
}

#[test]
fn pong_l() {
    test_file("PongL");
}

#[test]
fn rect() {
    test_file("Rect");
}

#[test]
fn rect_l() {
    test_file("RectL");
}
