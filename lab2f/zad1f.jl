#Author Bohdan Tkachenko 256630

using JuMP
import HiGHS

function read_data(file_name)
    open(file_name) do file
        lotniska = parse.(Int, split(readline(file)))
        firmy = parse.(Int, split(readline(file)))
        ceny = hcat([parse.(Int, split(line)) for line in eachline(file)]...)'
        return lotniska, firmy, ceny
    end
end

function solve_distribution(lotniska, firmy, ceny)
    L = 1:length(lotniska)
    F = 1:length(firmy)

    model = Model(HiGHS.Optimizer)

    @variable(model, 0 <= var[L, F])
    #Suma ile mają wszystkie firmy
    @constraint(model, [f in F], sum(var[:, f]) <= firmy[f])
    #Minimalne zapotrzebowanie paliwa
    @constraint(model, [l in L], sum(var[l, :]) >= lotniska[l])

    @objective(model, Min, sum(ceny[l, f] * var[l, f] for l in L, f in F))

    optimize!(model)

    return model
end

function display_results(model)
    println("\nCena ogólna: ", round(Int, objective_value(model)), "\n")
    L = 1:size(model[:var], 1)
    F = 1:size(model[:var], 2)
    for f in F, l in L
        println("Firma", f, " => Lotnisko", l, ": ", round(Int, value(model[:var][l, f])))
    end
    println("\n")
    for l in L
        println("Lotnisko", l, ": ", round(Int, value(sum(model[:var][l, :]))))
    end
    println("\n")
    for f in F
        println("Firma", f, ": ", round(Int, value(sum(model[:var][:, f]))))
    end
end

function main(file_name)
    lotniska, firmy, ceny = read_data(file_name)
    model = solve_distribution(lotniska, firmy, ceny)
    display_results(model)
end

main("data1.txt")
