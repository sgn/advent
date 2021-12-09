use std::io;

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut fish: [u64; 9] = [0; 9];
    io::stdin().read_line(&mut input);
    for f in input.trim_end().split(",")
        .map(|x| x.parse::<usize>().unwrap()) {
        fish[f] += 1;
    }

    for d in 0..18 {
        fish[(d + 7) % 9] += fish[d % 9];
    }
    println!("{}", fish.iter().fold(0, |s, v| s + v));
    for d in 18..80 {
        fish[(d + 7) % 9] += fish[d % 9];
    }
    println!("{}", fish.iter().fold(0, |s, v| s + v));
    for d in 80..256 {
        fish[(d + 7) % 9] += fish[d % 9];
    }
    println!("{}", fish.iter().fold(0, |s, v| s + v));
    Ok(())
}
