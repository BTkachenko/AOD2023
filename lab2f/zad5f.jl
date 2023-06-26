#Author Bohdan Tkachenko 256630


using JuMP
using Random
import HiGHS
include("dane5.jl")

function create_model(time_max, ppk, cpk, cph, maks_demand, mpk)
    ipk = ppk - cpk
    model = Model(HiGHS.Optimizer)
    @variable(model, 0 <= var[1:4, 1:3] <= time_max * 60, Int)
    #Czas dzialania maszyn
    @constraint(model, [i in 1:3], sum(var[:, i]) <= time_max * 60)
    # Ustawianie maksymalnych popytow
    @constraint(model, [i in 1:4], sum((var ./ mpk)[i, :]) <= maks_demand[i])
   
    @objective(model, Max, sum(ipk * (var ./ mpk)) - sum((var ./ 60) * cph))

    return model, var
end

function display_solution(model, var)
    solution_summary(model)

    println("\nTygodniowe czasy korzystania z maszyn, podzielone na produkty:")
    display(round.(Int, value.(var)))
    println("\nTygodniowe produkcje w kg:")
    for i in 1:4
        println("\tProdukt $(i): ", round(Int, value(sum((var ./ mpk)[i, :]))), "kg")
    end
    println("\nZysk ze sprzedaży tygodniowej produkcji: ", "\$", round(Int, objective_value(model)))
end

model,var = create_model(time_max, ppk, cpk, cph, maks_demand, mpk)
optimize!(model)
display_solution(model,var)
