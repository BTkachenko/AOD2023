#Author Bohdan Tkachenko 256630

using Random
using JuMP
import HiGHS
import LinearAlgebra

function generate_data(n)
    rng = MersenneTwister()

    time = zeros(n, n)
    cost = zeros(n, n)
    bilance = zeros(n)

    for i in 1:n, j in 1:min(n, i + 5)
        time[i, j] = rand(rng, 1:10)
        cost[i, j] = rand(rng, 1:10)
    end

    time_cap = n * 1.5
    town_start = 1
    town_end = n
    bilance[town_start] = 1
    bilance[town_end] = -1

    return time, cost, bilance, time_cap
end

function solve_shortest_path(n, time, cost, bilance, time_cap)
    model = Model(HiGHS.Optimizer)

    @variable(model, var[1:n, 1:n], Bin)

    #Cena nie moze byc 0
    @constraint(model, [i in 1:n, j in 1:n; cost[i, j] == 0], var[i, j] == 0)
    #Jedno wejscie 1 wyjscie
    @constraint(model, [i in 1:n], sum(var[i, :]) <= 1)
    @constraint(model, [i in 1:n], sum(var[:, i]) <= 1)
    #Bilans punktow
    @constraint(model, [i in 1:n], sum(var[i, :]) - sum(var[:, i]) == bilance[i])
    #Ograniczenie czasowe
    @constraint(model, sum(var .* time) <= time_cap)

    @objective(model, Min, sum(var .* cost))

    optimize!(model)

    return model
end

function display_results(model, time, cost)
    println("\nKoszt najkrótszej drogi: ", round(Int, objective_value(model)))
    println("Czas najkrótszej drogi: ", round(Int, value(sum(time .* model[:var]))))
    println("Ścieżka:")
    display(round.(Int, value.(model[:var])))
end

function main(n)
    time, cost, bilance, time_cap = generate_data(n)
    model = solve_shortest_path(n, time, cost, bilance, time_cap)
    display_results(model, time, cost)
end

main(10)


