
use std::path::PathBuf;

use clap::Parser;
use hw6::Translator;

#[derive(Parser)]
#[command()]
struct Cli {
    source: PathBuf,
    target: Option<PathBuf>,
}

fn main() {
    let cli = Cli::parse();

    let target = cli.target.unwrap_or(cli.source.with_extension("bin"));

    let code = std::fs::read_to_string(cli.source).unwrap();
    let mut translator = Translator::new();
    let translated = translator.translate(&code).unwrap();
    std::fs::write(target, translated).unwrap();
}

