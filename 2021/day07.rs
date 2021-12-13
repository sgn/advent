use std::io;

fn hyperbol_trisect(p: &Vec<usize>, begin: usize, end: usize,
           f: fn(&Vec<usize>, usize) -> usize) -> usize {
    let fbegin = f(&p, begin);
    if begin == end {
        return fbegin;
    }
    let fend = f(&p, end);
    if begin == end + 1 {
        return if fbegin < fend { fbegin } else { fend }
    }
    if begin == end + 2 {
        let fmid = f(&p, begin + 1);
        /* like a hyperbol */
        return if fbegin < fmid {
            fbegin
        } else if fmid < fend {
            fmid
        } else {
            fend
        }
    }
    let distance = end + 1 - begin;
    let onethird = distance / 3;
    let lmid = begin + onethird;
    let rmid = end - onethird;
    let first = hyperbol_trisect(&p, begin + 1, lmid, f);
    if fbegin < first {
        return fbegin
    }
    let second = hyperbol_trisect(&p, lmid + 1, rmid - 1, f);
    if first < second {
        return first
    }
    let third = hyperbol_trisect(&p, rmid, end - 1, f);
    if second < third {
        second
    } else if third < fend {
        third
    } else {
        fend
    }
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    io::stdin().read_line(&mut input);
    let mut positions : Vec<usize> = input
        .trim_end()
        .split(",")
        .map(|x| x.parse::<usize>().unwrap())
        .collect();
    positions.sort();
    let median = positions[positions.len() / 2 - 1];
    let cost = positions.iter()
        .map(|x| if x > &median { x - median } else { median -x })
        .sum::<usize>();
    println!("{}", cost);
    let inccost = hyperbol_trisect(
        &positions,
        0, positions.len() - 1,
        |a, p|
        a.iter().map(
            |x|
            if *x > p {(x - p) * (x + 1 - p) / 2}
            else {(p - x) * (p + 1 - x) / 2})
        .sum::<usize>());
    println!("{}", inccost);
    Ok(())
}
